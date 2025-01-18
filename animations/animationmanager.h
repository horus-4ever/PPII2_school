#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "animation.h"
#include "../datastructures/array.h"

struct Game;

typedef struct {
    ArrayList* animations;
    struct Game* game;
} AnimationManager;

/**
 * Initializes the animation system.
 * This function should be called before using any other animation-related functions.
 */
void initAnimations(void);

/**
 * Retrieves the global AnimationManager instance.
 * @return A pointer to the global AnimationManager instance.
 */
AnimationManager* getAnimationManager(void);

/**
 * Cleans up and quits the animation system.
 * This function should be called when the program is exiting.
 */
void Animations_Quit(void);

/**
 * Creates a new AnimationManager object.
 * @return A pointer to the newly created AnimationManager object.
 */
AnimationManager* AnimationManager_new();

/**
 * Sets the Game object associated with the AnimationManager.
 * @param manager A pointer to the AnimationManager object.
 * @param game A pointer to the Game object to be associated with the AnimationManager.
 */
void AnimationManager_setGame(AnimationManager* manager, struct Game* game);

/**
 * Updates all animations managed by the AnimationManager.
 * @param manager A pointer to the AnimationManager object.
 */
void AnimationManager_tick(AnimationManager* manager);

/**
 * Stops all animations managed by the AnimationManager.
 * @param manager A pointer to the AnimationManager object.
 */
void AnimationManager_stopAll(AnimationManager* manager);

/**
 * Stops a specific animation managed by the AnimationManager.
 * @param manager A pointer to the AnimationManager object.
 * @param animation A pointer to the Animation object to be stopped.
 */
void AnimationManager_stop(AnimationManager* manager, Animation* animation);

/**
 * Checks if a specific animation managed by the AnimationManager has finished playing.
 * @param manager A pointer to the AnimationManager object.
 * @param animation A pointer to the Animation object to be checked.
 * @return true if the animation has finished, false otherwise.
 */
bool AnimationManager_isFinished(AnimationManager* manager, Animation* animation);

/**
 * Renders all animations managed by the AnimationManager.
 * @param manager A pointer to the AnimationManager object.
 */
void AnimationManager_draw(AnimationManager* manager);

/**
 * Launches a new animation to be managed by the AnimationManager.
 * @param manager A pointer to the AnimationManager object.
 * @param animation A pointer to the Animation object to be launched.
 */
void AnimationManager_launch(AnimationManager* manager, Animation* animation);

/**
 * Deletes the AnimationManager object and frees associated resources.
 * @param manager A pointer to the AnimationManager object to be deleted.
 */
void AnimationManager_delete(AnimationManager* manager);

#endif