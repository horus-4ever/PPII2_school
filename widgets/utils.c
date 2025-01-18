#include "utils.h"

SDL_Rect fitIn(SDL_Rect src, SDL_Rect dest) {
    SDL_Rect result = src;
    if(src.x + src.w > dest.x + dest.w) {
        result.w = (dest.x + dest.w) - src.x;
    }
    if(src.y + src.h > dest.y + dest.h) {
        result.h = (dest.y + dest.h) - src.y;
    }
    return result;
}