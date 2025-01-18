#include "game.h"
#include "menu.h"
#include "fonctions_SDL.h"
#include "npc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void onParameterButtonClick(WDG_Button* button, void* param) {
    Game* game = param;
    Game_setState(game, GAME_PARAMETERS);
    play_sound(game->soundManager->sounds,INDEX_MENU);
}

void onPersoButtonClick(WDG_Button* button, void* param) {
    Game* game = param;
    Game_setState(game, GAME_CHARACTER_SELECTION);
    play_sound(game->soundManager->sounds,INDEX_MENU);
}

Game* Game_new(void) {
    Game* result = malloc(sizeof(Game));
    // init the window and renderer
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if(init(&window,&renderer,WIDTH,HEIGHT)){
        return NULL;
    }
    // map
    char* nom_fichier_background = "ressources/map/map_sol.csv";
    if(access(nom_fichier_background, F_OK) == -1){
        printf("Nom de fichier %s inexistant.\n",nom_fichier_background);fflush(stdout);
        return NULL;
    }
    Map* map = NULL;
    if(init_map(nom_fichier_background,&map,1)!=0){
        return NULL;
    }
    char* nom_fichier_object = "ressources/map/map_object.csv";
    if(access(nom_fichier_object, F_OK) == -1){
        printf("Nom d'image %s inexistant.\n",nom_fichier_object);fflush(stdout);
        return NULL;
    }
    int nb_objects, nb_type_objects;
    int* objects_width = NULL;
    int* objects_height = NULL;
    init_size_objects("ressources/map/size_objects.txt", &objects_width, &objects_height, &nb_objects, &nb_type_objects);
    Map* object = NULL;
    if(init_map(nom_fichier_object,&object,0)!=0){
        return NULL;
    }
    char* nom_image_fond = "ressources/images/background-Sheet.png";
    if(access(nom_image_fond, F_OK) == -1){
        printf("Nom d'image %s inexistant.\n",nom_image_fond);fflush(stdout);
        return NULL;
    }
    MapSDL* mapSDL = create_MapSDL(map, object, objects_width, objects_height, nb_objects, nb_type_objects);
    if(mapSDL == NULL){
        return NULL;
    }
    init_Blocks(mapSDL, load_image(nom_image_fond, renderer), renderer);
    // perso
    int spawn_x = WIDTH/2 - SIZE_PERSO/2 + OFFSET_PERSO_X + OFFSET_MAP_X;
    int spawn_y = HEIGHT/2 - SIZE_PERSO/2 + OFFSET_PERSO_Y + OFFSET_MAP_Y;
    Perso_SDL* perso = init_perso(spawn_x, spawn_y, SIZE_PERSO, SIZE_PERSO, 3, "ressources/images/vetements/woman/woman.png", renderer, 8, 5);
    if(perso == NULL){
        printf("Personnage non initialisé.\n");fflush(stdout);
        return NULL;
    }
    // button parameter
    WDG_Button* buttonParameters = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/parameters.png")), 85, 85);
    WDG_buttonSetPos(buttonParameters, WIDTH - 85 - 30, 30);
    WDG_buttonSetOnClick(buttonParameters, onParameterButtonClick, result);
    // button perso parameters
    WDG_Button* buttonPerso = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/persoMenu.png")), 85, 85);
    WDG_buttonSetPos(buttonPerso, WIDTH - 85*2 - 30*2, 30);
    WDG_buttonSetOnClick(buttonPerso, onPersoButtonClick, result);
    // clothes
    Character* character = Character_fromSdlPerso(perso);
    // construct the game object
    result->characterSelection = GameMenu_new(renderer);
    GameMenu_setGame(result->characterSelection, result);
    result->parameters = GameParameters_new(renderer);
    GameParameters_setGame(result->parameters, result);
    result->interpreter = Interpreter_new(character);
    Interpreter_setGame(result->interpreter, result);
    result->questManager = QuestManager_new();
    QuestManager_setGame(result->questManager, result);
    result->soundManager = SoundManager_new();
    SoundManager_setGame(result->soundManager, result);
    result->state = GAME_PARAMETERS;
    result->window = window;
    result->renderer = renderer;
    result->buttonParameters = buttonParameters;
    result->buttonPerso = buttonPerso;
    result->running = true;
    result->map = mapSDL;
    result->perso = perso;
    result->clothes_perso = NULL;
    result->player = character;
    result->currentTime = 0;
    result->timeSinceLastFootstepSound = 0;
    result->timeSinceLastChange = 0;
    result->current_footstep = 0;
    result->last_walking_sound = 0;
    return result;
}

void Game_setState(Game* game, GameState state) {
    game->state = state;
}

void _Game_handleKeydownEvent(Game* game, SDL_Event* event) {
    if (game->interpreter->state == INTERPRETER_STOPPED && event->key.keysym.sym == SDLK_m) {
        play_sound(game->soundManager->sounds, INDEX_MENU);
        Game_setState(game, GAME_CHARACTER_SELECTION);
    } else if(game->interpreter->state == INTERPRETER_STOPPED && event->key.keysym.sym == SDLK_i) {
        QuestManager_requestInteraction(game->questManager, game->perso);
    } else if(event->key.keysym.sym == SDLK_ESCAPE) {
        Game_setState(game, GAME_PARAMETERS);
    }
}

void _Game_handleKeyupEvent(Game* game, SDL_Event* event) {
    for (int i = 0; i < 4; i++) {
        reset_animation_perso(game->clothes_perso[i]);
    }
    reset_animation_perso(game->perso);
    reset_animation_npc(game->questManager->current->characters);
}

void _Game_handleEvents(Game* game, SDL_Event* event) {
    WDG_buttonDispatchEvent(game->buttonPerso, event);
    WDG_buttonDispatchEvent(game->buttonParameters, event);
    Interpreter_dispatchEvent(game->interpreter, event);
    switch (event->type) {
    case SDL_KEYDOWN:
        _Game_handleKeydownEvent(game, event);
        break;
    case SDL_KEYUP:
        _Game_handleKeyupEvent(game, event);
        break;
    default:
        break;
    }
}



void Game_dispatchEvents(Game* game, SDL_Event* event) {
    // handle top level events
    switch (event->type) {
    case SDL_QUIT:
        game->running = false;
        break;
    default:
        break;
    }
    // dispatch events following the state of the game
    switch (game->state) {
    case GAME_CHARACTER_SELECTION:
        GameMenu_dispatchEvents(game->characterSelection, event);
        break;
    case GAME_PARAMETERS:
        GameParameters_dispatchEvents(game->parameters, event);
        break;
    case GAME_PLAYING:
        _Game_handleEvents(game, event);
        break;
    case GAME_FINISHED:
        break;
    }
}

void Game_handleKeyboard(Game* game, Uint8* keystates) {
    if(game->interpreter->state != INTERPRETER_STOPPED || game->state != GAME_PLAYING) {
        return; // don't propagate events
    }
    Perso_SDL* perso = game->perso;
    Perso_SDL** clothes_perso = game->clothes_perso;
    MapSDL* mapSDL = game->map;
    Quest* quest = QuestManager_getCurrentQuest(game->questManager);
    if (game->timeSinceLastChange >= (1000.0 / 10)) {
        if (keystates[SDL_GetScancodeFromKey(SDLK_z)] || keystates[SDL_GetScancodeFromKey(SDLK_UP)]) {
            if (game->timeSinceLastFootstepSound > FOOTSTEP_DELAY) {
                play_footstep(game->soundManager->footstep, game->current_footstep);
                game->last_walking_sound = SDL_GetTicks();
            }
            update_position_joueur(perso, clothes_perso, perso->x, perso->y - PERSO_SPEED, mapSDL, quest->characters);
        }
        else if (keystates[SDL_GetScancodeFromKey(SDLK_s)] || keystates[SDL_GetScancodeFromKey(SDLK_DOWN)]) {
            if (game->timeSinceLastFootstepSound > FOOTSTEP_DELAY) {
                play_footstep(game->soundManager->footstep, game->current_footstep);
                game->last_walking_sound = SDL_GetTicks();
            }
            update_position_joueur(perso, clothes_perso, perso->x, perso->y + PERSO_SPEED, mapSDL, quest->characters);
        }
        else if (keystates[SDL_GetScancodeFromKey(SDLK_q)] || keystates[SDL_GetScancodeFromKey(SDLK_LEFT)]) {
            if (game->timeSinceLastFootstepSound > FOOTSTEP_DELAY) {
                play_footstep(game->soundManager->footstep, game->current_footstep);
                game->last_walking_sound = SDL_GetTicks();
            }
            update_position_joueur(perso, clothes_perso, perso->x - PERSO_SPEED, perso->y, mapSDL, quest->characters);
        }
        else if (keystates[SDL_GetScancodeFromKey(SDLK_d)] || keystates[SDL_GetScancodeFromKey(SDLK_RIGHT)]) {
            if (game->timeSinceLastFootstepSound > FOOTSTEP_DELAY) {
                play_footstep(game->soundManager->footstep, game->current_footstep);
                game->last_walking_sound = SDL_GetTicks();
            }
            update_position_joueur(perso, clothes_perso, perso->x + PERSO_SPEED, perso->y, mapSDL, quest->characters);
        }
        if (keystates[SDL_GetScancodeFromKey(SDLK_p)]) {
            int pos_x = perso->x, pos_y = perso->y;
            handleDoor(perso, clothes_perso, mapSDL);
            if(pos_x != perso->x || pos_y != perso->y){ // Si le joueur a bougé, alors il a traversé une porte
                play_sound(game->soundManager->sounds,INDEX_PORTE);
            }
        }
        else if (keystates[SDL_GetScancodeFromKey(SDLK_RETURN)]) {
            if (game->timeSinceLastFootstepSound > FOOTSTEP_DELAY) {
                play_footstep(game->soundManager->footstep, game->current_footstep);
                game->last_walking_sound = SDL_GetTicks();
            }
            update_position_joueur(perso, clothes_perso, perso->x, perso->y, mapSDL, quest->characters);
        }
        perso->lastMove = game->currentTime;
    }
}

void _Game_draw(Game* game) {
    // get the renderer and the main character
    SDL_Renderer* renderer = game->renderer;
    Perso_SDL* perso = game->perso;
    Perso_SDL** clothes_perso = game->clothes_perso;
    // draw the blocks
    render_blocks(game->map, game->renderer);
    NPC_SDL_RenderCopy(QuestManager_getCharacters(game->questManager), renderer, clothes_perso, perso);
    QuestManager_draw(game->questManager);
    WDG_drawButton(game->buttonParameters, renderer);
    WDG_drawButton(game->buttonPerso, renderer);
    // draw the player
    /* SDL_RenderCopy(renderer, perso->sprite, &perso->srcRect, &perso->dstRect);
    for(int i = 0 ; i < 4 ; i++){
        SDL_RenderCopy(renderer, clothes_perso[i]->sprite, &(clothes_perso[i]->srcRect), &(clothes_perso[i]->dstRect));
    } */
    Interpreter_draw(game->interpreter);
    // the animations
    AnimationManager_draw(getAnimationManager());
}

void Game_draw(Game* game) {
    SDL_RenderClear(game->renderer);
    switch (game->state) {
    case GAME_CHARACTER_SELECTION:
        GameMenu_draw(game->characterSelection);
        break;
    case GAME_PARAMETERS:
        GameParameters_draw(game->parameters);
        break;
    case GAME_PLAYING:
        _Game_draw(game);
        break;
    case GAME_FINISHED:
        AnimationManager_draw(getAnimationManager());
        break;
    default:
        break;
    }
}

void Game_quit(Game* game) {
    game->running = false;
}

void Game_update(Game* game) {
    game->currentTime = SDL_GetTicks();
    game->timeSinceLastChange = game->currentTime - game->perso->lastMove;
    game->timeSinceLastFootstepSound = game->currentTime - game->last_walking_sound;
    if(game->state == GAME_PLAYING) {
        QuestManager_adjustNpcPositionRelativeToMainPlayer(game->questManager, game->perso);
        AnimationManager_tick(getAnimationManager());
        Interpreter_tick(game->interpreter);
        if(game->interpreter->state == INTERPRETER_STOPPED) {
            QuestManager_update(game->questManager);
        }
    } else if(game->state == GAME_FINISHED) {
        AnimationManager_tick(getAnimationManager());
    }
}

void Game_delete(Game* game) {
    // call to sub delete functions
    Interpreter_delete(game->interpreter);
    QuestManager_delete(game->questManager);
    SoundManager_delete(game->soundManager);
    GameMenu_delete(game->characterSelection);
    GameParameters_delete(game->parameters);
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);

    WDG_deleteButton(game->buttonParameters);
    WDG_deleteButton(game->buttonPerso);

    free_mapSDL(game->map);
    Character_delete(game->player);
    if(game->clothes_perso != NULL) {
        free_clothes(game->clothes_perso);
    }
    // free the pointer
    free(game);
}