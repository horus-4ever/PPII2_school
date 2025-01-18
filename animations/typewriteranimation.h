#ifndef TYPEWRITERTANIMATION_H
#define TYPEWRITERTANIMATION_H

#include "animation.h"
#include "../widgets/textarea.h"
#include <SDL2/SDL.h>

/**
 * Structure representing the context for a typewriter animation.
 */
typedef struct {
    WDG_TextArea* textarea; /**< Pointer to the text area associated with the typewriter animation context. */
    UTF8_String* text; /**< Pointer to the UTF8_String containing the text for the typewriter animation. */
} TypewriterAnimationContext;

/**
 * Creates a new TypewriterAnimationContext object.
 * @param text A pointer to the WDG_TextArea containing the text for the typewriter animation.
 * @return A pointer to the newly created TypewriterAnimationContext object.
 */
TypewriterAnimationContext* TypewriterAnimationContext_new(WDG_TextArea* text);

/**
 * Deletes the TypewriterAnimationContext object and frees associated resources.
 * @param context A pointer to the TypewriterAnimationContext object to be deleted.
 */
void TypewriterAnimationContext_delete(TypewriterAnimationContext* context);

/**
 * Creates a new TypewriterAnimation object.
 * @param context A pointer to the TypewriterAnimationContext object containing the animation context.
 * @return A pointer to the newly created TypewriterAnimation object.
 */
Animation* TypewriterAnimation_new(TypewriterAnimationContext* context);

/**
 * Updates the typewriter animation state by advancing to the next frame.
 * @param animation A pointer to the TypewriterAnimation object to be updated.
 */
void TypewriterAnimation_tick(Animation* animation);

/**
 * Renders the current frame of the typewriter animation.
 * @param animation A pointer to the TypewriterAnimation object to be rendered.
 * @param renderer The SDL_Renderer object used for rendering.
 */
void TypewriterAnimation_draw(Animation* animation, SDL_Renderer* renderer);

/**
 * Deletes the TypewriterAnimation object and frees associated resources.
 * @param animation A pointer to the TypewriterAnimation object to be deleted.
 */
void TypewriterAnimation_delete(Animation* animation);


#endif