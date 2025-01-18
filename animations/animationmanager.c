#include "animationmanager.h"
#include <stdlib.h>
#include "../game.h"

static AnimationManager* _animationManager = NULL;

AnimationManager* AnimationManager_new() {
    AnimationManager* result = malloc(sizeof(AnimationManager));
    result->animations = ArrayList_new(Animation_delete);
    return result;
}

void AnimationManager_setGame(AnimationManager* manager, Game* game) {
    manager->game = game;
}

void initAnimations(void) {
    _animationManager = AnimationManager_new();
}

AnimationManager* getAnimationManager(void) {
    return _animationManager;
}

void Animations_Quit(void) {
    if(_animationManager != NULL) {
        AnimationManager_delete(_animationManager);
    }
}

void AnimationManager_tick(AnimationManager* manager) {
    for(size_t i = 0 ; i < ArrayList_length(manager->animations) ; i += 1) {
        Animation* animation = ArrayList_get(manager->animations, i);
        if(Animation_isFinished(animation)) {
            ArrayList_remove(manager->animations, animation);
        } else {
            Animation_tick(animation);
        }
    }
}

void AnimationManager_draw(AnimationManager* manager) {
    for(size_t i = 0 ; i < ArrayList_length(manager->animations) ; i += 1) {
        Animation* animation = ArrayList_get(manager->animations, i);
        if(!Animation_isFinished(animation)) {
            Animation_draw(animation, manager->game->renderer);
        }
    }
}

void AnimationManager_stopAll(AnimationManager* manager) {
    ArrayList_clear(manager->animations);
}

void AnimationManager_stop(AnimationManager* manager, Animation* animation) {
    Animation* result = ArrayList_find(manager->animations, animation, NULL);
    if(result != NULL) {
        Animation_stop(animation);
        ArrayList_remove(manager->animations, animation);
    }
}

bool AnimationManager_isFinished(AnimationManager* manager, Animation* animation) {
    Animation* result = ArrayList_find(manager->animations, animation, NULL);
    if(result == NULL) {
        return true;
    } else {
        return Animation_isFinished(result);
    }
}

void AnimationManager_launch(AnimationManager* manager, Animation* animation) {
    ArrayList_append(manager->animations, animation);
}

void AnimationManager_delete(AnimationManager* manager) {
    ArrayList_delete(manager->animations);
    free(manager);
}