#include "itemanimation.h"
#include "../fonctions_SDL.h"
#include "../items.h"
#include <math.h>

#define M_PI acos(-1.0)

ItemAnimationContext* ItemAnimationContext_new(SDL_Texture* texture) {
    ItemAnimationContext* result = malloc(sizeof(ItemAnimationContext));
    result->texture = texture;
    return result;
}

Animation* ItemAnimation_new(ItemAnimationContext* context) {
    return Animation_new(FPS * 3, false, ItemAnimation_tick, ItemAnimation_draw, ItemAnimation_delete, context);
}

void ItemAnimation_tick(Animation* animation) {
    
}

void ItemAnimation_draw(Animation* animation, SDL_Renderer* renderer) {
    ItemAnimationContext* context = animation->context;
    SDL_Texture* texture = context->texture;
    // calculate coordinates
    int centerx = WIDTH / 2;
    int centery = HEIGHT / 2;
    float rate = (float) animation->count / animation->nbFrames;
    SDL_Color grey = {0, 0, 0, 0};
    SDL_Rect src;
    float angle = 0;
    int width = 24 * (5 + rate * 5);
    int height = 24 * (5 + rate * 5);
    SDL_Rect dest = {centerx - width / 2, centery - height / 2, width, height};
    if(rate < 0.5) {
        src = (SDL_Rect) {.x = 0, .y = 0, .w = SIZE_ITEMS, .h = SIZE_ITEMS};
        grey.a = (rate * 2 * 200);
        angle = 360.0 * rate * 2;
    } else {
        src = (SDL_Rect) {.x = SIZE_ITEMS, .y = 0, .w = SIZE_ITEMS, .h = SIZE_ITEMS};
        grey.a = 200;
    }
    // draw the animation items
    SDL_SetRenderDrawColor(renderer, grey.r, grey.g, grey.b, grey.a);
    SDL_RenderFillRect(renderer, NULL);
    SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, 0);
}

void ItemAnimation_delete(Animation* animation) {
    ItemAnimationContext_delete(animation->context);
}

void ItemAnimationContext_delete(ItemAnimationContext* context) {
    free(context);
}