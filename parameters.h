#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "widgets/widgets.h"
#include "sound.h"

struct Game;

/**
 * @brief Represents the game parameters.
 */
typedef struct GameParameters {
    WDG_Button* buttonSound;       ///< Button for sound settings.
    WDG_Button* quitButton;        ///< Button for quitting game.
    WDG_Button* buttonStart;       ///< Button for starting game.
    WDG_Button* buttonContinue;    ///< Button for continuing game.
    SDL_Texture* background;       ///< Background texture.
    struct Game* game;             ///< Reference to the game.
    
    // Cursors for sound settings
    Cursor* cursor_music;          ///< Cursor for music volume.
    Cursor* cursor_footsteps;      ///< Cursor for footsteps volume.
    Cursor* cursor_sounds;         ///< Cursor for other sounds volume.
    int isDraggingCursor;          ///< Flag indicating if a cursor is being dragged.
    
    bool firstTime;                ///< Flag indicating if it's the first time running the game.
} GameParameters;

/**
 * @brief Creates a new GameParameters object.
 * @param renderer The SDL renderer.
 * @return A pointer to the newly created GameParameters object.
 */
GameParameters* GameParameters_new(SDL_Renderer* renderer);

/**
 * @brief Sets the game instance for the game parameters.
 * @param parameters The GameParameters object.
 * @param game The game instance.
 */
void GameParameters_setGame(GameParameters* parameters, struct Game* game);

/**
 * @brief Dispatches events to the game parameters.
 * @param parameters The GameParameters object.
 * @param event The SDL event.
 */
void GameParameters_dispatchEvents(GameParameters* parameters, SDL_Event* event);

/**
 * @brief Draws the game parameters.
 * @param parameters The GameParameters object.
 */
void GameParameters_draw(GameParameters* parameters);

/**
 * @brief Deletes the GameParameters object and frees its resources.
 * @param parameters The GameParameters object to delete.
 */
void GameParameters_delete(GameParameters* parameters);

#endif