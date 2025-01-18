#ifndef WDG_STACKFRAME_H
#define WDG_STACKFRAME_H

#include "widget.h"
#include "../datastructures/hashmap.h"
#include "../utf8strings/utf8strings.h"
#include <SDL2/SDL.h>

typedef struct {
    WDG_Widget base;
    void* current;
    HashMap* frames;
} WDG_Stackframe;

WDG_Stackframe* WDG_createStackFrame(void);
void WDG_stackframeAdd(WDG_Stackframe* frame, UTF8_String* name, void* value);
void WDG_stackframeSet(WDG_Stackframe* frame, UTF8_String* name);
void WDG_stackframeDisplay(WDG_Stackframe* frame, SDL_Renderer* renderer);

void WDG_stackframeDelete(WDG_Stackframe* frame);

#endif