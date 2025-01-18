#include "stackframe.h"

WDG_Stackframe* WDG_createStackFrame(void) {
    WDG_Stackframe* result = malloc(sizeof(WDG_Stackframe));
    result->current = NULL;
    result->frames = HashMap_new(UTF8_delete, free, UTF8_equal, UTF8_hash);
    return result;
}

void WDG_stackframeAdd(WDG_Stackframe* frame, UTF8_String* name, void* value) {
    HashMap_set(frame->frames, name, value);
    frame->current = value;
}

void WDG_stackframeSet(WDG_Stackframe* frame, UTF8_String* name) {
    frame->current = HashMap_get(frame->frames, name);
}

void WDG_stackframeDisplay(WDG_Stackframe* frame, SDL_Renderer* renderer) {

}

void WDG_stackframeDelete(WDG_Stackframe* frame) {
    HashMap_delete(frame->frames);
    free(frame);
}