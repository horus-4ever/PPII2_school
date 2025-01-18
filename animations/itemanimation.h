#ifndef ITEMANIMATION_H
#define ITEMANIMATION_H

#include "animation.h"
#include <SDL2/SDL.h>

/**
 * Structure representing the context for an item animation.
 */
typedef struct {
    SDL_Texture* texture; /**< Pointer to the SDL_Texture associated with the item animation context. */
} ItemAnimationContext;

/**
 * Creates a new ItemAnimationContext object.
 * @param texture A pointer to the SDL_Texture containing the texture for the item animation.
 * @return A pointer to the newly created ItemAnimationContext object.
 */
ItemAnimationContext* ItemAnimationContext_new(SDL_Texture* texture);

/**
 * Deletes the ItemAnimationContext object and frees associated resources.
 * @param context A pointer to the ItemAnimationContext object to be deleted.
 */
void ItemAnimationContext_delete(ItemAnimationContext* context);

/**
 * Creates a new ItemAnimation object.
 * @param context A pointer to the ItemAnimationContext object containing the animation context.
 * @return A pointer to the newly created ItemAnimation object.
 */
Animation* ItemAnimation_new(ItemAnimationContext* context);

/**
 * Updates the item animation state by advancing to the next frame.
 * @param animation A pointer to the ItemAnimation object to be updated.
 */
void ItemAnimation_tick(Animation* animation);

/**
 * Renders the current frame of the item animation.
 * @param animation A pointer to the ItemAnimation object to be rendered.
 * @param renderer The SDL_Renderer object used for rendering.
 */
void ItemAnimation_draw(Animation* animation, SDL_Renderer* renderer);

/**
 * Deletes the ItemAnimation object and frees associated resources.
 * @param animation A pointer to the ItemAnimation object to be deleted.
 */
void ItemAnimation_delete(Animation* animation);

#endif