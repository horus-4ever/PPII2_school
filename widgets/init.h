#ifndef WDG_INIT_H
#define WDG_INIT_H

#include "font.h"

void WDG_Init(void);
void WDG_Quit(void);
TTF_Font* WDG_getFont(const char* name, int fontSize);

#endif