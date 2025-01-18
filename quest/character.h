#ifndef CHARACTER_H
#define CHARACTER_H

#include "../utf8strings/utf8strings.h"
#include "../datastructures/array.h"
#include "../perso_SDL.h"
#include "../questparser/compiler.h"
#include "../animations/animation.h"
#include <stdbool.h>

/**
 * Structure representing a character.
 */
typedef struct {
    UTF8_String* name; /**< Pointer to the name of the character. */
    Bytecode* bytecode; /**< Pointer to the bytecode of the character. */
    Perso_SDL* sdlPerso; /**< Pointer to the Perso_SDL representing the character. */
    UTF8_String* skinName; /**< Pointer to the name of the character's skin. */
    Animation* animation; /**< Pointer to the animation of the character. */
    bool talked; /**< Flag indicating if the character has been talked to. */
    bool canInteract; /**< Flag indicating if the character can be interacted with. */
} Character;

/**
 * Creates a new character.
 * @param name A pointer to the UTF8_String containing the name of the character.
 * @param skin A pointer to the UTF8_String containing the skin name of the character.
 * @param spawnx The x-coordinate of the character's spawn position.
 * @param spawny The y-coordinate of the character's spawn position.
 * @param orientation The orientation of the character.
 * @param bytecode A pointer to the bytecode of the character.
 * @param renderer A pointer to the SDL_Renderer used for rendering.
 * @return A pointer to the newly created Character object.
 */
Character* Character_new(UTF8_String* name, UTF8_String* skin, int spawnx, int spawny, int orientation, Bytecode* bytecode, SDL_Renderer* renderer);

/**
 * Creates a character from an existing Perso_SDL object.
 * @param perso A pointer to the Perso_SDL representing the character.
 * @return A pointer to the newly created Character object.
 */
Character* Character_fromSdlPerso(Perso_SDL* perso);

/**
 * Sets the name of the character.
 * @param character A pointer to the Character object.
 * @param name A pointer to the UTF8_String containing the name of the character.
 */
void Character_setName(Character* character, UTF8_String* name);

/**
 * Deletes the Character object and frees associated resources.
 * @param character A pointer to the Character object to be deleted.
 */
void Character_delete(Character* character);

/**
 * Updates the position of the character.
 * @param character A pointer to the Character object.
 * @param x The new x-coordinate of the character.
 * @param y The new y-coordinate of the character.
 */
void Character_updatePosition(Character* character, int x, int y);

/**
 * Draws the character.
 * @param character A pointer to the Character object.
 * @param renderer A pointer to the SDL_Renderer used for rendering.
 */
void Character_draw(Character* character, SDL_Renderer* renderer);

/**
 * Checks if the character has been talked to.
 * @param character A pointer to the Character object.
 * @return true if the character has been talked to, false otherwise.
 */
bool Character_talkedWith(Character* character);

/**
 * Makes the character follow a path.
 * @param character A pointer to the Character object.
 * @param path A pointer to the UTF8_String containing the path for the character to follow.
 * @param renderer A pointer to the SDL_Renderer used for rendering.
 */
void Character_followPath(Character* character, UTF8_String* path, SDL_Renderer* renderer);

/**
 * Adjusts the character's position relative to the main player.
 * @param character A pointer to the Character object.
 * @param mc A pointer to the Perso_SDL representing the main player.
 */
void Character_adjustRelativeToMainPlayer(Character* character, Perso_SDL* mc);

#endif