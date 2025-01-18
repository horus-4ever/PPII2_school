#include "animation.h"
#include <stdlib.h>

Animation* Animation_new(size_t frames, bool repeat, void (*tick)(struct Animation* animation), void (*draw)(struct Animation* animation, SDL_Renderer* renderer), void (*delete)(struct Animation* animation), void* context) {
    Animation* result = malloc(sizeof(Animation));
    result->context = context;
    result->count = 0;
    result->nbFrames = frames;
    result->tick = tick;
    result->draw = draw;
    result->delete = delete;
    result->state = ANIMATION_RUNNING;
    result->repeat = repeat;
    return result;
}

void Animation_tick(Animation* animation) {
    animation->count += 1;
    if(animation->tick != NULL) {
        animation->tick(animation);
    }
    if(animation->count >= animation->nbFrames) {
        if(animation->repeat) {
            animation->count = 0;
        } else {
            animation->state = ANIMATION_FINISHED;
        }
    }
}

void Animation_draw(Animation* animation, SDL_Renderer* renderer) {
    if(animation->draw != NULL) {
        animation->draw(animation, renderer);
    }
}

void Animation_stop(Animation* animation) {
    animation->state = ANIMATION_FINISHED;
}

void Animation_pause(Animation* animation) {
    animation->state = ANIMATION_PAUSED;
}

bool Animation_isFinished(Animation* animation) {
    return animation->state == ANIMATION_FINISHED;
}

void Animation_delete(Animation* animation) {
    animation->delete(animation);
    free(animation);
}