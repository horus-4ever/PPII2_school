#ifndef GAME_H
#define GAME_H

#include "menu.h"
#include "parameters.h"
#include "interpreter.h"
#include "quest/questmanager.h"
#include "animations/animationmanager.h"
#include "sound.h"
#include <SDL2/SDL.h>

/**
 * @brief Represents the different states of the game.
 */
typedef enum {
    GAME_PARAMETERS,            ///< Game parameters state.
    GAME_CHARACTER_SELECTION,   ///< Character selection state.
    GAME_PLAYING,               ///< Game playing state.
    GAME_FINISHED               ///< Game finished state.
} GameState;

/**
 * @brief Represents the main game structure.
 */
typedef struct Game {
    GameMenu* characterSelection;   ///< The character selection menu.
    GameParameters* parameters;     ///< The game parameters.
    SDL_Window* window;             ///< The SDL window.
    SDL_Renderer* renderer;         ///< The SDL renderer.
    WDG_Button* buttonParameters;   ///< The button for accessing game parameters.
    WDG_Button* buttonPerso;        ///< The button for accessing character selection.
    // Logical elements
    GameState state;                ///< The current state of the game.
    Interpreter* interpreter;       ///< The interpreter for game logic.
    QuestManager* questManager;     ///< The manager for game quests.
    SoundManager* soundManager;     ///< The manager for game sounds.
    MapSDL* map;                    ///< The game map.
    bool running;                   ///< Flag indicating if the game is running.
    // Player related elements
    Perso_SDL** clothes_perso;     ///< The clothing options for the player character.
    Perso_SDL* perso;               ///< The player character.
    Character* player;              ///< The player's character.
    // Time related
    Uint32 currentTime;             ///< The current time in the game.
    Uint32 timeSinceLastChange;     ///< Time since the last state change.
    Uint32 timeSinceLastFootstepSound;  ///< Time since the last footstep sound.
    int current_footstep;           ///< Current footstep sound index.
    Uint32 last_walking_sound;      ///< Last time a walking sound was played.
} Game;

/**
 * @brief Creates a new Game object.
 * @return A pointer to the newly created Game object.
 */
Game* Game_new(void);

/**
 * @brief Sets the state of the game.
 * @param game The Game object.
 * @param state The new state to set.
 */
void Game_setState(Game* game, GameState state);

/**
 * @brief Dispatches events to the game.
 * @param game The Game object.
 * @param event The SDL event to dispatch.
 */
void Game_dispatchEvents(Game* game, SDL_Event* event);

/**
 * @brief Handles keyboard events for the game.
 * @param game The Game object.
 * @param keyboard The keyboard state.
 */
void Game_handleKeyboard(Game* game, Uint8* keyboard);

/**
 * @brief Draws the game on the screen.
 * @param game The Game object.
 */
void Game_draw(Game* game);

/**
 * @brief Quits the game and frees its resources.
 * @param game The Game object to quit.
 */
void Game_quit(Game* game);

/**
 * @brief Updates the game logic.
 * @param game The Game object.
 */
void Game_update(Game* game);

/**
 * @brief Deletes a Game object and frees its resources.
 * @param game The Game object to delete.
 */
void Game_delete(Game* game);

#endif