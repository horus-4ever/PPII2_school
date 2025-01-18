#ifndef ANIMATION_H
#define ANIMATION_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum {
    ANIMATION_RUNNING,
    ANIMATION_PAUSED,
    ANIMATION_FINISHED
} AnimationState;

typedef struct Animation {
    size_t count;
    size_t nbFrames;
    AnimationState state;
    bool repeat;
    void* context;
    void (*tick)(struct Animation* animation);
    void (*draw)(struct Animation* animation, SDL_Renderer* renderer);
    void (*delete)(struct Animation* animation);
} Animation;

/**
 * Creates a new animation object
 * @param frames The total number of frames in the animation
 * @param repeat Boolean indicating whether the animation should repeat
 * @param tick Pointer to the function responsible for updating the animation state
 * @param draw Pointer to the function responsible for drawing the animation
 * @param delete Pointer to the function responsible for deleting the animation
 * @param context A pointer to additional context data needed for the animation (optional, can be NULL)
 * @return A pointer to the newly created Animation object
 */
Animation* Animation_new(size_t frames, bool repeat, void (*tick)(struct Animation* animation), void (*draw)(struct Animation* animation, SDL_Renderer* renderer), void (*delete)(struct Animation* animation), void* context);

/**
 * Updates the animation state by one frame
 * @param animation Pointer to the Animation object
 * @return None
 */
void Animation_tick(Animation* animation);

/**
 * Draws the current frame of the animation on the specified SDL_Renderer
 * @param animation Pointer to the Animation object
 * @param renderer Pointer to the SDL_Renderer object on which to draw the animation
 * @return None
 */
void Animation_draw(Animation* animation, SDL_Renderer* renderer);

/**
 * Stops the animation, resetting it to the beginning
 * @param animation Pointer to the Animation object
 * @return None
 */
void Animation_stop(Animation* animation);

/**
 * Pauses the animation at its current frame
 * @param animation Pointer to the Animation object
 * @return None
 */
void Animation_pause(Animation* animation);

/**
 * Checks if the animation has finished playing
 * @param animation Pointer to the Animation object
 * @return True if the animation has finished, False otherwise
 */
bool Animation_isFinished(Animation* animation);

/**
 * Deletes the animation object and frees allocated memory
 * @param animation Pointer to the Animation object
 * @return None
 */
void Animation_delete(Animation* animation);

#endif