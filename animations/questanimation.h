#ifndef SUBQUESTANIMATION_H
#define SUBQUESTANIMATION_H

#include "animation.h"
#include "../widgets/textarea.h"
#include <SDL2/SDL.h>

/**
 * Structure representing the context for a quest animation.
 */
typedef struct {
    WDG_TextArea* textarea; /**< Pointer to the text area associated with the quest animation context. */
} QuestAnimationContext;

/**
 * Creates a new QuestAnimationContext object.
 * @param text A pointer to the UTF8_String containing the text for the quest animation.
 * @return A pointer to the newly created QuestAnimationContext object.
 */
QuestAnimationContext* QuestAnimationContext_new(UTF8_String* text);

/**
 * Deletes the QuestAnimationContext object and frees associated resources.
 * @param context A pointer to the QuestAnimationContext object to be deleted.
 */
void QuestAnimationContext_delete(QuestAnimationContext* context);

/**
 * Creates a new QuestAnimation object.
 * @param context A pointer to the QuestAnimationContext object containing the animation context.
 * @return A pointer to the newly created QuestAnimation object.
 */
Animation* QuestAnimation_new(QuestAnimationContext* context);

/**
 * Updates the quest animation state by advancing to the next frame.
 * @param animation A pointer to the QuestAnimation object to be updated.
 */
void QuestAnimation_tick(Animation* animation);

/**
 * Renders the current frame of the quest animation.
 * @param animation A pointer to the QuestAnimation object to be rendered.
 * @param renderer The SDL_Renderer object used for rendering.
 */
void QuestAnimation_draw(Animation* animation, SDL_Renderer* renderer);

/**
 * Deletes the QuestAnimation object and frees associated resources.
 * @param animation A pointer to the QuestAnimation object to be deleted.
 */
void QuestAnimation_delete(Animation* animation);


#endif