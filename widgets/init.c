#include "init.h"
#include <stdbool.h>

static bool __init = false;
static WDG_FontManager* __manager = NULL;

void WDG_Init(void) {
    if(__init == false) {
        __manager = WDG_FontManager_new();
        __init = true;
        WDG_FontManager_get(__manager, DEFAULT_FONT, DEFAULT_FONTSIZE);
    }
}

void WDG_Quit(void) {
    WDG_FontManager_delete(__manager);
}

TTF_Font* WDG_getFont(const char* name, int fontSize) {
    return WDG_FontManager_get(__manager, name, fontSize);
}