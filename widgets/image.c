#include "image.h"
#include "utils.h"
#include <SDL2/SDL_image.h>

WDG_Image* WDG_createImage(UTF8_String* filename) {
    SDL_Surface* surface = IMG_Load(UTF8_str(filename));
    if(surface == NULL){
        WDG_SDLABORT(EXIT_FAILURE);
    }
    int width = surface->w;
    int height = surface->h;
    WDG_Image* result = malloc(sizeof(WDG_Image));
    WDG_Padding padding = {0, 0, 0, 0};
    SDL_Rect position = {0, 0, width, height};
    WDG_initWidget(&result->base, position, padding);
    result->image = surface;
    result->imageName = filename;
    return result;
}

void WDG_imageSetPos(WDG_Image* image, int x, int y) {
    WDG_widgetSetPos(&image->base, x, y);
}

void WDG_imageResize(WDG_Image* image, int width, int height) {
    image->base.position.w = width;
    image->base.position.h = height;
}

void WDG_drawImage(WDG_Image* image, SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image->image);
    SDL_Rect outerRect = WDG_WidgetGetOuterRect(&image->base);
    SDL_RenderCopy(renderer, texture, NULL, &outerRect);
    SDL_DestroyTexture(texture);
}

void WDG_deleteImage(WDG_Image* image) {
    UTF8_delete(image->imageName);
    SDL_FreeSurface(image->image);
    free(image);
}