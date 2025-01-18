#include "textarea.h"
#include "colors.h"
#include "../animations/animationmanager.h"
#include "../animations/typewriteranimation.h"

WDG_TextArea* WDG_createTextArea(UTF8_String* text, TTF_Font *textFont, int width, int height) {
    WDG_TextArea* result = calloc(1, sizeof(WDG_TextArea));
    WDG_Padding padding = {WDG_TEXTAREA_PADDING, WDG_TEXTAREA_PADDING, WDG_TEXTAREA_PADDING, WDG_TEXTAREA_PADDING};
    SDL_Rect position = {0, 0, width + WDG_TEXTAREA_PADDING, height + WDG_TEXTAREA_PADDING};
    WDG_initWidget(&result->base, position, padding);
    result->text = text;
    result->textFont = textFont;
    result->fontColor = WDG_GREY;
    result->animation = NULL;
    return result;
}

void WDG_textareaSetPos(WDG_TextArea* textarea, int x, int y) {
    WDG_widgetSetPos(&textarea->base, x, y);
}

void WDG_textareaSetText(WDG_TextArea* textarea, UTF8_String* string) {
    if(textarea->text != NULL) {
        UTF8_delete(textarea->text);
    }
    textarea->text = string;
}

void WDG_textareaSetFontColor(WDG_TextArea* textarea, WDG_Color color) {
    textarea->fontColor = color;
}

void WDG_deleteTextArea(WDG_TextArea* textarea) {
    UTF8_delete(textarea->text);
    if(textarea->base.texture != NULL) {
        SDL_DestroyTexture(textarea->base.texture);
    }
    if(textarea->animation != NULL) {
        AnimationManager_stop(getAnimationManager(), textarea->animation);
    }
    free(textarea);
}

void WDG_drawTextArea(WDG_TextArea* textarea, SDL_Renderer* renderer) {
    if(WDG_widgetIsHidden(&textarea->base)) {
        return;
    }
    WDG_drawWidget(&textarea->base, renderer);
    
    if(UTF8_length(textarea->text) == 0) {
        return;
    }
    // create the good rect
    SDL_Rect innerRect = WDG_WidgetGetInnerRect(&textarea->base);
    int wrapLength = innerRect.w;
    // render the text
    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(textarea->textFont, UTF8_str(textarea->text), textarea->fontColor, wrapLength);
    if(surface == NULL) {
        WDG_SDLABORT(EXIT_FAILURE);
    }
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surface);
    if(text == NULL) {
        WDG_SDLABORT(EXIT_FAILURE);
    }
    
    SDL_Rect textRect = {.x = innerRect.x, .y = innerRect.y};
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
    SDL_Rect toGet = {.x = 0, .y = 0, .w = innerRect.w, .h = innerRect.h};
    SDL_Rect toDisplay = fitIn(textRect, innerRect);
    SDL_RenderCopy(renderer, text, &toGet, &toDisplay);

    // free
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text);
}

void WDG_animateTextArea(WDG_TextArea* textarea) {
    AnimationManager* manager = getAnimationManager();
    if(textarea->animation != NULL) {
        AnimationManager_stop(manager, textarea->animation);
        textarea->animation = NULL;
    }
    TypewriterAnimationContext* context = TypewriterAnimationContext_new(textarea);
    Animation* animation = TypewriterAnimation_new(context);
    AnimationManager_launch(manager, animation);
    textarea->animation = animation;
}