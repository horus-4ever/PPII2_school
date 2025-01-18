#ifndef NPCANIMATION_H
#define NPCANIMATION_H

#include "animation.h"
#include "../utf8strings/utf8strings.h"
#include "../quest/character.h"
#include <SDL2/SDL.h>

/**
 * Structure representing the context for an NPC (Non-Player Character) animation.
 */
typedef struct {
    Character* character; /**< Pointer to the Character associated with the NPC animation context. */
    UTF8_String* path; /**< Pointer to the UTF8_String containing the path for the NPC animation. */
} NPCAnimationContext;

/**
 * Creates a new NPCAnimationContext object.
 * @param character A pointer to the Character representing the NPC.
 * @param path A pointer to the UTF8_String containing the path for the NPC animation.
 * @return A pointer to the newly created NPCAnimationContext object.
 */
NPCAnimationContext* NPCAnimationContext_new(Character* character, UTF8_String* path);

/**
 * Deletes the NPCAnimationContext object and frees associated resources.
 * @param context A pointer to the NPCAnimationContext object to be deleted.
 */
void NPCAnimationContext_delete(NPCAnimationContext* context);

/**
 * Creates a new NPCAnimation object.
 * @param context A pointer to the NPCAnimationContext object containing the animation context.
 * @return A pointer to the newly created NPCAnimation object.
 */
Animation* NPCAnimation_new(NPCAnimationContext* context);

/**
 * Updates the NPC animation state by advancing to the next frame.
 * @param animation A pointer to the NPCAnimation object to be updated.
 */
void NPCAnimation_tick(Animation* animation);

/**
 * Renders the current frame of the NPC animation.
 * @param animation A pointer to the NPCAnimation object to be rendered.
 * @param renderer The SDL_Renderer object used for rendering.
 */
void NPCAnimation_draw(Animation* animation, SDL_Renderer* renderer);

/**
 * Deletes the NPCAnimation object and frees associated resources.
 * @param animation A pointer to the NPCAnimation object to be deleted.
 */
void NPCAnimation_delete(Animation* animation);


#endif