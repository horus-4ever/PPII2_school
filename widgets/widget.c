#include "widget.h"
#include "colors.h"
#include <stdbool.h>

void WDG_initWidget(WDG_Widget* widget, SDL_Rect position, WDG_Padding padding) {
    widget->position = position;
    widget->padding = padding;
    widget->state = WDG_NORMAL;
    widget->texture = NULL;
    widget->backgroundColor = WDG_TRANSPARENT;
    widget->callbacks.mouseEnter = NULL;
    widget->callbacks.mouseLeave = NULL;
    widget->callbacks.mouseEnterArgs = NULL;
    widget->callbacks.mouseLeaveArgs = NULL;
    WDG_initBorder(&widget->borders, WDG_NONE, (SDL_Color){0, 0, 0, 255}, 10);
}

void WDG_initBorder(WDG_Borders* borders, WDG_Directions where, SDL_Color color, int borderSize) {
    borders->borders = where;
    borders->borderColor = color;
    borders->borderSize = borderSize;
}

void WDG_widgetSetFocus(WDG_Widget* widget) {
    WDG_widgetSetState(widget, WDG_FOCUSED);
}

SDL_Rect WDG_WidgetGetOuterRect(const WDG_Widget* widget) {
    return widget->position;
}

SDL_Rect WDG_WidgetGetInnerRect(const WDG_Widget* widget) {
    return (SDL_Rect) {
        .x = widget->position.x + widget->padding.left,
        .y = widget->position.y + widget->padding.top,
        .w = widget->position.w - widget->padding.left - widget->padding.right,
        .h = widget->position.h - widget->padding.top - widget->padding.down
    };
}

WDG_WidgetState WDG_widgetGetState(const WDG_Widget* widget) {
    return widget->state;
}

bool WDG_widgetIs(WDG_Widget* widget, WDG_WidgetState state) {
    return widget->state & state;
}

bool WDG_widgetIsFocused(WDG_Widget* widget) {
    return WDG_widgetIs(widget, WDG_FOCUSED);
}

bool WDG_widgetIsHidden(WDG_Widget* widget) {
    return WDG_widgetIs(widget, WDG_HIDDEN);
}

bool WDG_widgetIsHovered(WDG_Widget* widget) {
    return WDG_widgetIs(widget, WDG_MOUSEOVER);
}

void WDG_widgetSetOnMouseEnter(WDG_Widget* widget, WDG_Callback callback, void* args) {
    widget->callbacks.mouseEnter = callback;
    widget->callbacks.mouseEnterArgs = args;
}

void WDG_widgetSetOnMouseLeave(WDG_Widget* widget, WDG_Callback callback, void* args) {
    widget->callbacks.mouseLeave = callback;
    widget->callbacks.mouseLeaveArgs = args;
}

void WDG_widgetSetPos(WDG_Widget* widget, int x, int y) {
    widget->position.x = x;
    widget->position.y = y;
}

void WDG_widgetSetBorder(WDG_Widget* widget, WDG_Directions borders) {
    widget->borders.borders = borders;
}

void WDG_widgetSetBorderColor(WDG_Widget* widget, SDL_Color color) {
    widget->borders.borderColor = color;
}

void WDG_widgetSetBorderSize(WDG_Widget* widget, int borderSize) {
    widget->borders.borderSize = borderSize;
}

void WDG_widgetSetBackgroundColor(WDG_Widget* widget, SDL_Color color) {
    widget->backgroundColor = color;
}

bool WDG_widgetCollide(const WDG_Widget* widget, int x, int y) {
    return (x >= widget->position.x) && (x <= widget->position.x + widget->position.w)
        && (y >= widget->position.y) && (y <= widget->position.y + widget->position.h);
}

void _WDG_widgetHandleMouseMotionEvent(WDG_Widget* widget, SDL_Event* event) {
    bool collision = WDG_widgetCollide(widget, event->motion.x, event->motion.y);
    if(collision && !(widget->state & WDG_MOUSEOVER)) {
        widget->state |= WDG_MOUSEOVER;
        if(widget->callbacks.mouseEnter != NULL) {
            widget->callbacks.mouseEnter(widget, widget->callbacks.mouseEnterArgs);
        }
    } else if(!collision && (widget->state & WDG_MOUSEOVER)) {
        widget->state ^= WDG_MOUSEOVER;
        if(widget->callbacks.mouseLeave != NULL) {
            widget->callbacks.mouseLeave(widget, widget->callbacks.mouseLeaveArgs);
        }
    }
}

void _WDG_widgetHandleMouseButtonDown(WDG_Widget* widget, SDL_Event* event) {
    bool collision = WDG_widgetCollide(widget, event->motion.x, event->motion.y);
    if(collision) {
        widget->state |= WDG_FOCUSED;
    } else {
        widget->state &= ~WDG_FOCUSED;
    }
}

void WDG_widgetHandleEvent(WDG_Widget* widget, SDL_Event* event) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            _WDG_widgetHandleMouseMotionEvent(widget, event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            _WDG_widgetHandleMouseButtonDown(widget, event);
            break;
        default:
            break;
    }
}

void WDG_widgetSetState(WDG_Widget* widget, WDG_WidgetState state) {
    widget->state = state;
}

void WDG_widgetToggleState(WDG_Widget* widget, WDG_WidgetState state) {
    widget->state ^= state;
}

void WDG_drawWidget(WDG_Widget* widget, SDL_Renderer* renderer) {
    SDL_Rect outerRect = WDG_WidgetGetOuterRect(widget);
    // render the background
    SDL_Color backgroundColor = widget->backgroundColor;
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &outerRect);
    // render the borders
    SDL_Color color = widget->borders.borderColor;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const int borderSize = widget->borders.borderSize;
    if(widget->borders.borders & WDG_UP) {
        SDL_Rect upRect = {.x = outerRect.x - borderSize, .y = outerRect.y - borderSize, .w = outerRect.w + 2 * borderSize, .h = borderSize};
        SDL_RenderFillRect(renderer, &upRect);
    }
    if(widget->borders.borders & WDG_DOWN) {
        SDL_Rect downRect = {.x = outerRect.x - borderSize, .y = outerRect.y + outerRect.h, .w = outerRect.w + 2 * borderSize, .h = borderSize};
        SDL_RenderFillRect(renderer, &downRect);
    }
    if(widget->borders.borders & WDG_LEFT) {
        SDL_Rect leftRect = {.x = outerRect.x - borderSize, .y = outerRect.y - borderSize, .w = borderSize, .h = outerRect.h + 2 * borderSize};
        SDL_RenderFillRect(renderer, &leftRect);
    }
    if(widget->borders.borders & WDG_RIGHT) {
        SDL_Rect rightRect = {.x = outerRect.x + outerRect.w, .y = outerRect.y - borderSize, .w = borderSize, .h = outerRect.h + 2 * borderSize};
        SDL_RenderFillRect(renderer, &rightRect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}