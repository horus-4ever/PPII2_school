#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "fonctions_SDL.h"
#include "perso_SDL.h"
#include "npc.h"
#include "widgets/widgets.h"
#include "utf8strings/utf8strings.h"
#include "map_SDL.h"
#include "map.h"
#include "unistd.h"
#include "datastructures/linkedlist.h"
#include "datastructures/hashmap.h"
#include "datastructures/array.h"
#include "quest/quest.h"
#include "interpreter.h"
#include "game.h"
#include "sound.h"
#include "items.h"
#include "animations/animationmanager.h"
#include "animations/endanimation.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int statut = EXIT_FAILURE;

    if(0 != SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    init_audio();
    TTF_Init();
    WDG_Init();
    initAnimations();

    Game* game = Game_new();
    if(game == NULL) {
        goto Quit;
    }
    AnimationManager_setGame(getAnimationManager(), game);
    // initialise SDL for transparent display
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    UTF8_String* path = UTF8_from("ressources/quests/quest1");
    QuestManager_load(game->questManager, path);
    UTF8_delete(path);

    Uint32 lastTime = 0;


    while(game->running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            Game_dispatchEvents(game, &event);
        }
        // keyboard events for the game
        Uint8* keystates = (Uint8*) SDL_GetKeyboardState(NULL);

        Game_handleKeyboard(game, keystates);
        
        Game_update(game);

        SDL_RenderClear(game->renderer);
        Game_draw(game);
        SDL_RenderPresent(game->renderer);
        // FPS
        Uint32 current = SDL_GetTicks();
        if(current - lastTime < 1000.0 / FPS) {
            Uint32 delay = current - lastTime;
            SDL_Delay(delay);
        }
        lastTime = current;
    }

    statut = EXIT_SUCCESS;
Quit:
    Game_delete(game);
    WDG_Quit();
    Animations_Quit();
    SDL_Quit();
    return statut;
}
