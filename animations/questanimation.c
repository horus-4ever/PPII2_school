#include "questanimation.h"
#include "../fonctions_SDL.h"
#include "../items.h"
#include "../widgets/init.h"
#include <math.h>

#define M_PI acos(-1.0)

QuestAnimationContext* QuestAnimationContext_new(UTF8_String* text) {
    QuestAnimationContext* result = malloc(sizeof(QuestAnimationContext));
    WDG_TextArea* textarea = WDG_createTextArea(UTF8_copy(text), WDG_getFont(DEFAULT_FONT, 60), WIDTH - 20, HEIGHT - 20);
    result->textarea = textarea;
    return result;
}

Animation* QuestAnimation_new(QuestAnimationContext* context) {
    return Animation_new(3 * FPS, false, QuestAnimation_tick, QuestAnimation_draw, QuestAnimation_delete, context);
}

void QuestAnimation_tick(Animation* animation) {
    
}

void QuestAnimation_draw(Animation* animation, SDL_Renderer* renderer) {
    QuestAnimationContext* context = animation->context;
    // calculate coordinates
    int centery = HEIGHT / 2;
    float rate = (float) animation->count / animation->nbFrames;
    SDL_Color yellow = {252,211,100,255};
    WDG_Color purple = {183 * (rate), 139 * (rate), 254 * (rate), 255};
    WDG_textareaSetFontColor(context->textarea, purple);
    WDG_textareaSetPos(context->textarea, 20, centery);
    // draw the animation items
    SDL_SetRenderDrawColor(renderer, yellow.r, yellow.g, yellow.b, yellow.a);
    SDL_RenderFillRect(renderer, NULL);
    WDG_drawTextArea(context->textarea, renderer);
}

void QuestAnimation_delete(Animation* animation) {
    QuestAnimationContext_delete(animation->context);
}

void QuestAnimationContext_delete(QuestAnimationContext* context) {
    WDG_deleteTextArea(context->textarea);
    free(context);
}