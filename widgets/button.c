#include "button.h"
#include "colors.h"
#include <stdlib.h>

WDG_Button* WDG_createTextButton(UTF8_String* text, TTF_Font* textFont, int width, int height) {
    WDG_Button* result = calloc(1, sizeof(WDG_Button));
    WDG_Padding padding = {WDG_BUTTON_PADDING, WDG_BUTTON_PADDING, WDG_BUTTON_PADDING, WDG_BUTTON_PADDING};
    SDL_Rect position = {0, 0, width + WDG_BUTTON_PADDING, height + WDG_BUTTON_PADDING};
    WDG_initWidget(&result->base, position, padding);
    result->text = WDG_createTextArea(text, textFont, width, height);
    result->callback = NULL;
    result->callbackArgs = NULL;
    result->image = NULL;
    WDG_widgetSetBorder(&result->base, WDG_ALL);
    WDG_widgetSetBorderColor(&result->base, WDG_WHITE);
    WDG_widgetSetBorderSize(&result->base, 1);
    return result;
}

WDG_Button* WDG_createImageButton(WDG_Image* image, int width, int height) {
    WDG_imageResize(image, width, height);

    WDG_Button* result = calloc(1, sizeof(WDG_Button));
    WDG_Padding padding = {0, 0, 0, 0};
    SDL_Rect position = {0, 0, width, height};
    WDG_initWidget(&result->base, position, padding);
    result->callback = NULL;
    result->callbackArgs = NULL;
    result->image = image;
    return result;
}

void WDG_buttonSetFontColor(WDG_Button* button, WDG_Color color) {
    WDG_textareaSetFontColor(button->text, color);
}

void WDG_buttonSetPos(WDG_Button* button, int x, int y) {
    WDG_widgetSetPos(&button->base, x, y);
    if(button->image != NULL) {
        WDG_imageSetPos(button->image, x, y);
    } else {
        WDG_textareaSetPos(button->text, x, y);
    }
}

void WDG_buttonSetOnClick(WDG_Button* button, WDG_ButtonCallback callback, void* args) {
    button->callback = callback;
    button->callbackArgs = args;
}

UTF8_String* WDG_buttonGetText(WDG_Button* button) {
    return button->text->text;
}

void WDG_deleteButton(WDG_Button* button) {
    if(button->image == NULL) {
        WDG_deleteTextArea(button->text);
    } else {
        WDG_deleteImage(button->image);
    }
    free(button);
}

void WDG_drawButton(WDG_Button* button, SDL_Renderer* renderer) {
    if(WDG_buttonIs(button, WDG_HIDDEN)) {
        return;
    }
    // draw the base widget
    WDG_drawWidget(&button->base, renderer);

    if(button->image == NULL) {
        // draw the button
        WDG_drawTextArea(button->text, renderer);
    }
    else {
        WDG_drawImage(button->image, renderer);
    }
}

WDG_Widget* WDG_buttonGetBase(WDG_Button* button) {
    return &button->base;
}

void WDG_buttonDispatchEvent(WDG_Button* button, SDL_Event* event) {
    if(WDG_widgetIsHidden(&button->base)) {
        return;
    }
    WDG_widgetHandleEvent(&button->base, event);
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            bool collision = WDG_widgetCollide(&button->base, event->button.x, event->button.y);
            if(collision && button->callback != NULL) {
                button->callback(button, button->callbackArgs);
            }
            break;
        default:
            break;
    }
}