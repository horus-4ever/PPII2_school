#include "textinput.h"
#include "textarea.h"
#include "colors.h"

WDG_TextInput* WDG_createTextInput(TTF_Font *textFont, int width, int height) {
    WDG_TextInput* result = calloc(1, sizeof(WDG_TextInput));
    WDG_Padding padding = {WDG_TEXTINPUT_PADDING, WDG_TEXTINPUT_PADDING, WDG_TEXTINPUT_PADDING, WDG_TEXTINPUT_PADDING};
    SDL_Rect position = {0, 0, width + WDG_TEXTINPUT_PADDING, height + WDG_TEXTINPUT_PADDING};
    WDG_initWidget(&result->base, position, padding);
    result->text = UTF8_empty();
    result->textFont = textFont;
    result->maxLength = WDG_TEXTINPUT_MAXLENGTH;
    result->fontColor = WDG_BLACK;
    return result;
}

void WDG_textinputSetMaxLength(WDG_TextInput* textinput, size_t maxLength) {
    if(maxLength > WDG_TEXTINPUT_MAXLENGTH) {
        maxLength = WDG_TEXTINPUT_MAXLENGTH;
    }
    textinput->maxLength = maxLength;
}

void WDG_textinputSetPos(WDG_TextInput* textinput, int x, int y) {
    WDG_widgetSetPos(&textinput->base, x, y);
}

void WDG_textinputSetFontColor(WDG_TextInput* textinput, WDG_Color color) {
    textinput->fontColor = color;
}

void WDG_deleteTextInput(WDG_TextInput* textinput) {
    UTF8_delete(textinput->text);
    if(textinput->base.texture != NULL) {
        SDL_DestroyTexture(textinput->base.texture);
    }
    free(textinput);
}

void WDG_drawTextInput(WDG_TextInput* textinput, SDL_Renderer* renderer) {
    if(WDG_widgetIsHidden(&textinput->base)) {
        return;
    }
    WDG_drawWidget(&textinput->base, renderer);

    // constants
    const int fontHeight = TTF_FontHeight(textinput->textFont);
    const int cursor = WDG_TEXTINPUT_CURSOR_SIZE;

    SDL_Rect innerRect = WDG_WidgetGetInnerRect(&textinput->base);
    SDL_Rect toDisplayRect;

    if(textinput->text->length > 0) { // if the text is not empty
        // render the text and create a texture from it
        SDL_Surface* surface = TTF_RenderUTF8_Blended(textinput->textFont, UTF8_str(textinput->text), textinput->fontColor);
        if(surface == NULL) {
            WDG_SDLABORT(EXIT_FAILURE);
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if(textTexture == NULL) {
            WDG_SDLABORT(EXIT_FAILURE);
        }
        // get the values of different rect
        SDL_Rect textRect;
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = innerRect.x;
        textRect.y = (innerRect.y + innerRect.h / 2) - (textRect.h / 2);
        // construct the display rectangle
        toDisplayRect = (SDL_Rect) {
            .x = innerRect.x,
            .w = innerRect.w - cursor,
            .y = innerRect.y, 
            .h = innerRect.h
        };
        toDisplayRect = fitIn(textRect, toDisplayRect);
        // construct the part of the text to get (x-scrolling)
        int xShift = textRect.w - toDisplayRect.w;
        SDL_Rect toGet = {.x = xShift, .y = 0, .w = innerRect.w - cursor, .h = innerRect.h};

        SDL_RenderCopy(renderer, textTexture, &toGet, &toDisplayRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(textTexture);
    } else { // in case the text is empty
        toDisplayRect = (SDL_Rect){.x = innerRect.x, .w = 0, .y = (innerRect.y + innerRect.h / 2) - fontHeight / 2, .h = 0};
    }
    SDL_SetRenderDrawColor(renderer, textinput->fontColor.r, textinput->fontColor.g, textinput->fontColor.b, textinput->fontColor.a);
    if(WDG_widgetIsFocused(&textinput->base)) {
        SDL_Rect cursorRect = {toDisplayRect.x + toDisplayRect.w, toDisplayRect.y + fontHeight, cursor, 2};
        SDL_Color fontColor = textinput->fontColor;
        SDL_SetRenderDrawColor(renderer, fontColor.r, fontColor.g, fontColor.b, fontColor.a);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void WDG_textinputEventTextAppend(WDG_TextInput* textinput, const char* str) {
    size_t textLength = UTF8_length(textinput->text);
    size_t strLength = UTF8_utf8length(str);
    if(textLength + strLength > textinput->maxLength) {
        return;
    }
    UTF8_append(textinput->text, str);
}

void WDG_textinputEventErase(WDG_TextInput* textinput) {
    UTF8_String* newString = UTF8_pop(textinput->text);
    UTF8_delete(textinput->text);
    textinput->text = newString;
}

void WDG_textinputDispatchEvent(WDG_TextInput* textinput, SDL_Event* event) {
    if(WDG_widgetIsHidden(&textinput->base)) {
        return;
    }
    // dispatch events to the underlying base widget
    WDG_widgetHandleEvent(&textinput->base, event);
    // in case the widget is focused, we can begin to enter text
    if(WDG_widgetIsFocused(&textinput->base)) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }
    // append / erase text
    if(WDG_widgetIsFocused(&textinput->base) && event->type == SDL_TEXTINPUT) {
        WDG_textinputEventTextAppend(textinput, event->text.text);
    } else if(WDG_widgetIsFocused(&textinput->base) && event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE) {
        WDG_textinputEventErase(textinput);
    }
}