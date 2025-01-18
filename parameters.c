#include "parameters.h"
#include "game.h"
#include <stdlib.h>

void onQuit(WDG_Button* button, void* param) {
    GameParameters* parameters = param;
    Game* game = parameters->game;
    Game_quit(game);
}

void onClickToggleMenu(WDG_Button* button, void* data) {
    ToggleParams* params = (ToggleParams*)data;
    *params->parameters =  (*params->parameters + 1) %2;
    play_sound(params->sons,INDEX_MENU);
}

void onContinue(WDG_Button* button, void* param) {
    GameParameters* parameters = param;
    Game* game = parameters->game;
    Game_setState(game, GAME_PLAYING);
    play_sound(game->soundManager->sounds,INDEX_MENU);
}

void onStart(WDG_Button* button, void* param) {
    GameParameters* parameters = param;
    Game* game = parameters->game;
    parameters->firstTime = false;
    Game_setState(game, GAME_CHARACTER_SELECTION);
    play_sound(game->soundManager->sounds,INDEX_MENU);
}

void onClickToggleSound(WDG_Button* button, void* param){
    GameParameters* parameters = param;
    Game* game = parameters->game;
    SoundManager* soundManager = game->soundManager;
    soundManager->sound =  (soundManager->sound + 1) %2;
    if (soundManager->sound == 1){
        WDG_deleteImage(button->image);
        button->image = WDG_createImage(UTF8_from("ressources/images/sound.png"));
        WDG_buttonSetPos(button, 30, 30);
        change_volume_music(128);
        change_volume_sons(soundManager->sounds,128);
        change_volume_footstep(soundManager->footstep,128);
        parameters->cursor_music->dstCursor.x = MAX_CURSOR;
        parameters->cursor_footsteps->dstCursor.x = MAX_CURSOR;
        parameters->cursor_sounds->dstCursor.x = MAX_CURSOR;
    }
    else{
        WDG_deleteImage(button->image);
        button->image = WDG_createImage(UTF8_from("ressources/images/mute.png"));
        WDG_buttonSetPos(button, 30, 30);
        change_volume_music(0);
        change_volume_sons(soundManager->sounds,0);
        change_volume_footstep(soundManager->footstep,0);
        parameters->cursor_music->dstCursor.x = MIN_CURSOR;
        parameters->cursor_footsteps->dstCursor.x = MIN_CURSOR;
        parameters->cursor_sounds->dstCursor.x = MIN_CURSOR;

    }
}

GameParameters* GameParameters_new(SDL_Renderer* renderer) {
    /* Creation of the graphical gidgets */
    GameParameters* result = malloc(sizeof(GameParameters));
    // start button
    WDG_Button* buttonStart = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/button_start.png")), 277, 85);
    WDG_buttonSetPos(buttonStart, WIDTH/2 - 277/2, 720);
    WDG_buttonSetOnClick(buttonStart, onStart, result);
    // continue button
    WDG_Button* buttonContinue = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/button_continue.png")), 391, 85);
    WDG_buttonSetPos(buttonContinue, WIDTH/2 - 391/2, 720);
    WDG_buttonSetOnClick(buttonContinue, onContinue, result);
    // quit button
    WDG_Button* quitButton = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/exit.png")), 85, 85);
    WDG_buttonSetPos(quitButton, WIDTH - 30 - 85, 30);
    WDG_buttonSetOnClick(quitButton, onQuit, result);
    // sound button
    WDG_Button* buttonSound = WDG_createImageButton(WDG_createImage(UTF8_from("ressources/images/sound.png")), 85, 85);
    WDG_buttonSetPos(buttonSound, 30, 30);
    WDG_buttonSetOnClick(buttonSound, onClickToggleSound, result);
    // the background
    SDL_Texture* background = load_image("ressources/images/background1.png", renderer);
    if (!background) {
        printf("Erreur lors de la création du background 1\n");fflush(stdout);
        return NULL;
    }
    /* assignement to the result */
    result->buttonContinue = buttonContinue;
    result->buttonSound = buttonSound;
    result->buttonStart = buttonStart;
    result->quitButton = quitButton;
    result->background = background;
    result->cursor_music = init_cursor(0, renderer);
    result->cursor_footsteps = init_cursor(1, renderer);
    result->cursor_sounds = init_cursor(2, renderer);
    result->isDraggingCursor = 0;
    result->firstTime = true;
    return result;
}

void GameParameters_setGame(GameParameters* parameters, Game* game) {
    parameters->game = game;
}

void GameParameters_dispatchEvents(GameParameters* parameters, SDL_Event* event) {
    WDG_buttonDispatchEvent(parameters->quitButton, event);
    if(parameters->firstTime) {
        WDG_buttonDispatchEvent(parameters->buttonStart, event);
    } else {
        WDG_buttonDispatchEvent(parameters->buttonContinue, event);
    }
    WDG_buttonDispatchEvent(parameters->buttonSound, event);
    // get the sound manager
    SoundManager* soundManager = parameters->game->soundManager;
    // handle events
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            int mouseX = event->button.x;
            int mouseY = event->button.y;

            if (mouseY >= 160 && mouseY <= 190) {
                clickCursor(parameters->cursor_music, mouseX, &parameters->isDraggingCursor, 1, NULL);
            }
            if (mouseY >= 202 && mouseY <= 232) {
                clickCursor(parameters->cursor_footsteps, mouseX, &parameters->isDraggingCursor, 2, soundManager->footstep);
            }
            if (mouseY >= 244 && mouseY <= 274) {
                clickCursor(parameters->cursor_sounds, mouseX, &parameters->isDraggingCursor, 3, soundManager->sounds);
            }
            int new_sound = calcul_volume(mouseX);
            if (new_sound != 0 && soundManager->sound == 0)
            {
                soundManager->sound = 1;
                WDG_deleteImage(parameters->buttonSound->image);
                parameters->buttonSound->image = WDG_createImage(UTF8_from("ressources/images/sound.png"));
                WDG_buttonSetPos(parameters->buttonSound, 30, 30);
            }
            if (parameters->cursor_music->dstCursor.x == MIN_CURSOR && parameters->cursor_footsteps->dstCursor.x == MIN_CURSOR && parameters->cursor_sounds->dstCursor.x == MIN_CURSOR)
            {
                soundManager->sound = 0;
                WDG_deleteImage(parameters->buttonSound->image);
                parameters->buttonSound->image = WDG_createImage(UTF8_from("ressources/images/mute.png"));
                WDG_buttonSetPos(parameters->buttonSound, 30, 30);
            }
        }
    }
    else if (event->type == SDL_MOUSEMOTION && parameters->isDraggingCursor != 0)
    {
        int cursorX = event->motion.x - 8;
        if (cursorX < MIN_CURSOR)
        {
            cursorX = MIN_CURSOR;
        }
        else if (cursorX > MAX_CURSOR)
        {
            cursorX = MAX_CURSOR;
        }
        int new_sound = calcul_volume(cursorX);
        if (new_sound != 0 && soundManager->sound == 0)
        {
            soundManager->sound = 1;
            WDG_deleteImage(parameters->buttonSound->image);
            parameters->buttonSound->image = WDG_createImage(UTF8_from("ressources/images/sound.png"));
            WDG_buttonSetPos(parameters->buttonSound, 30, 30);
        }
        switch (parameters->isDraggingCursor)
        {
        case 1:
            parameters->cursor_music->dstCursor.x = cursorX;
            change_volume_music(new_sound);
            break;
        case 2:
            parameters->cursor_footsteps->dstCursor.x = cursorX;
            change_volume_footstep(soundManager->footstep, new_sound);
            break;
        case 3:
            parameters->cursor_sounds->dstCursor.x = cursorX;
            change_volume_sons(soundManager->sounds, new_sound);
            break;
        default:
            break;
        }
        if (parameters->cursor_music->dstCursor.x == MIN_CURSOR && parameters->cursor_footsteps->dstCursor.x == MIN_CURSOR && parameters->cursor_sounds->dstCursor.x == MIN_CURSOR)
        {
            soundManager->sound = 0;
            WDG_deleteImage(parameters->buttonSound->image);
            parameters->buttonSound->image = WDG_createImage(UTF8_from("ressources/images/mute.png"));
            WDG_buttonSetPos(parameters->buttonSound, 30, 30);
        }
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            parameters->isDraggingCursor = 0;
        }
    }
}

void GameParameters_draw(GameParameters* parameters) {
    SDL_Renderer* renderer = parameters->game->renderer;
    SDL_RenderCopy(renderer, parameters->background, NULL, NULL);
    WDG_drawButton(parameters->quitButton, renderer);
    if(parameters->firstTime) {
        WDG_drawButton(parameters->buttonStart, renderer);
    } else {
        WDG_drawButton(parameters->buttonContinue, renderer);
    }
    WDG_drawButton(parameters->buttonSound, renderer);
    render_cursor(parameters->cursor_music, renderer);
    render_cursor(parameters->cursor_footsteps, renderer);
    render_cursor(parameters->cursor_sounds, renderer);
}

void GameParameters_delete(GameParameters* parameters) {
    WDG_deleteButton(parameters->buttonSound);
    WDG_deleteButton(parameters->quitButton);
    WDG_deleteButton(parameters->buttonStart);
    WDG_deleteButton(parameters->buttonContinue);
    SDL_DestroyTexture(parameters->background);
    // destroy cursors
    free_cursor(parameters->cursor_music);
    free_cursor(parameters->cursor_footsteps);
    free_cursor(parameters->cursor_sounds);
    // free the pointer
    free(parameters);
}