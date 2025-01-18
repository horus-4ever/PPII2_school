#include "endanimation.h"
#include "../fonctions_SDL.h"
#include "../items.h"
#include "../widgets/init.h"
#include <math.h>

#define M_PI acos(-1.0)

EndAnimationContext* EndAnimationContext_new(UTF8_String* text) {
    EndAnimationContext* result = malloc(sizeof(EndAnimationContext));
    WDG_TextArea* textarea = WDG_createTextArea(text, WDG_getFont(DEFAULT_FONT, 60), WIDTH - 20, HEIGHT - 20);
    result->textarea = textarea;
    return result;
}

Animation* EndAnimation_new(EndAnimationContext* context) {
    return Animation_new(FPS * 5, true, EndAnimation_tick, EndAnimation_draw, EndAnimation_delete, context);
}

void EndAnimation_tick(Animation* animation) {

}

void EndAnimation_draw(Animation* animation, SDL_Renderer* renderer) {
    EndAnimationContext* context = animation->context;
    // calculate coordinates
    float rate = (float) animation->count / animation->nbFrames;
    SDL_Color yellow = {252,211,100,255};
    WDG_Color purple;
    if (rate < 0.5) {
        purple = (WDG_Color) {183 * (rate * 2), 139 * (rate * 2), 254 * (rate * 2), 255};
    } else {
        purple = (WDG_Color) {183 * ((1 - rate) * 2), 139 * ((1 - rate) * 2), 254 * ((1 - rate) * 2), 255};
    }
    SDL_Texture* logo = load_image("ressources/images/logo.png", renderer);
    SDL_Rect* dst = malloc(sizeof(SDL_Rect));
    dst->x = 0;
    dst->y = 0;
    dst->h = HEIGHT;
    dst->w = WIDTH;
    WDG_textareaSetFontColor(context->textarea, purple);
    WDG_textareaSetPos(context->textarea, WIDTH/2-80, HEIGHT-200);
    // draw the animation items
    SDL_SetRenderDrawColor(renderer, yellow.r, yellow.g, yellow.b, yellow.a);
    SDL_RenderFillRect(renderer, NULL);
    WDG_drawTextArea(context->textarea, renderer);
    SDL_RenderCopy(renderer, logo, NULL, dst);
    SDL_DestroyTexture(logo);
    free(dst);
}

void EndAnimation_delete(Animation* animation) {
    EndAnimationContext_delete(animation->context);
}

void EndAnimationContext_delete(EndAnimationContext* context) {
    WDG_deleteTextArea(context->textarea);
    free(context);
}