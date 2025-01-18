#include "typewriteranimation.h"
#include "../fonctions_SDL.h"
#include <stdlib.h>

TypewriterAnimationContext* TypewriterAnimationContext_new(WDG_TextArea* textarea) {
    TypewriterAnimationContext* result = malloc(sizeof(TypewriterAnimationContext));
    result->textarea = textarea;
    result->text = UTF8_copy(textarea->text);
    return result;
}

void TypewriterAnimationContext_delete(TypewriterAnimationContext* context) {
    UTF8_delete(context->text);
    free(context);
}

Animation* TypewriterAnimation_new(TypewriterAnimationContext* context) {
    size_t length = UTF8_length(context->text);
    int stepDuration = 0.05 * FPS;
    return Animation_new(length * stepDuration, false, TypewriterAnimation_tick, TypewriterAnimation_draw, TypewriterAnimation_delete, context);
}

void TypewriterAnimation_tick(Animation* animation) {
    TypewriterAnimationContext* context = animation->context;
    int stepDuration = 0.05 * FPS;
    size_t length = animation->count / stepDuration;
    UTF8_Slice* slice = UTF8_slice(context->text, 0, length);
    WDG_textareaSetText(context->textarea, UTF8_toString(slice));
    UTF8_sliceDelete(slice);
}

void TypewriterAnimation_draw(Animation* animation, SDL_Renderer* renderer) {
    TypewriterAnimationContext* context = animation->context;
    WDG_drawTextArea(context->textarea, renderer);
}

void TypewriterAnimation_delete(Animation* animation) {
    TypewriterAnimationContext_delete(animation->context);
}