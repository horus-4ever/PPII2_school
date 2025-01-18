#ifndef ENDANIMATION_H
#define ENDANIMATION_H

#include "animation.h"
#include "../widgets/textarea.h"
#include <SDL2/SDL.h>

/**
 * Structure representing the context for an end animation.
 */
typedef struct {
    WDG_TextArea* textarea; /**< Pointer to the text area associated with the end animation context. */
} EndAnimationContext;

/**
 * Creates a new EndAnimationContext object.
 * @param text A pointer to the UTF8_String containing the text for the end animation.
 * @return A pointer to the newly created EndAnimationContext object.
 */
EndAnimationContext* EndAnimationContext_new(UTF8_String* text);

/**
 * Deletes the EndAnimationContext object and frees associated resources.
 * @param context A pointer to the EndAnimationContext object to be deleted.
 */
void EndAnimationContext_delete(EndAnimationContext* context);

/**
 * Creates a new EndAnimation object.
 * @param context A pointer to the EndAnimationContext object containing the animation context.
 * @return A pointer to the newly created EndAnimation object.
 */
Animation* EndAnimation_new(EndAnimationContext* context);

/**
 * Updates the end animation state by advancing to the next frame.
 * @param animation A pointer to the EndAnimation object to be updated.
 */
void EndAnimation_tick(Animation* animation);

/**
 * Renders the current frame of the end animation.
 * @param animation A pointer to the EndAnimation object to be rendered.
 * @param renderer The SDL_Renderer object used for rendering.
 */
void EndAnimation_draw(Animation* animation, SDL_Renderer* renderer);

/**
 * Deletes the EndAnimation object and frees associated resources.
 * @param animation A pointer to the EndAnimation object to be deleted.
 */
void EndAnimation_delete(Animation* animation);

#endif