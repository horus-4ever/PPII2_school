#ifndef WDG_UTILS_H
#define WDG_UTILS_H

#include <SDL2/SDL.h>

SDL_Rect fitIn(SDL_Rect dest, SDL_Rect src);
typedef SDL_Color WDG_Color;

#define WDG_SDLABORT(status) { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "(%s:%d): %s\n", __FILE__, __LINE__, SDL_GetError()); SDL_Quit(); exit(status); }

#endif