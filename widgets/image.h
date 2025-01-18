#ifndef WDG_IMAGE_H
#define WDG_IMAGE_H

#include <SDL2/SDL.h>
#include "widget.h"
#include "../utf8strings/utf8strings.h"

/**
 * @brief Represents an image widget.
 */
typedef struct {
    WDG_Widget base;          ///< The base widget.
    SDL_Surface* image;       ///< The SDL surface containing the image.
    UTF8_String* imageName;   ///< The name of the image file.
} WDG_Image;

/**
 * @brief Creates a new image widget from a file.
 * @param filename The filename of the image file.
 * @return A pointer to the newly created WDG_Image object, or NULL on failure.
 */
WDG_Image* WDG_createImage(UTF8_String* filename);

/**
 * @brief Sets the position of an image widget.
 * @param image The WDG_Image object to modify.
 * @param x The new x-coordinate of the top-left corner of the image.
 * @param y The new y-coordinate of the top-left corner of the image.
 */
void WDG_imageSetPos(WDG_Image* image, int x, int y);

/**
 * @brief Resizes an image widget.
 * @param image The WDG_Image object to resize.
 * @param width The new width of the image.
 * @param height The new height of the image.
 */
void WDG_imageResize(WDG_Image* image, int width, int height);

/**
 * @brief Draws the image on the specified SDL_Renderer.
 * @param image The WDG_Image object to be drawn.
 * @param renderer The SDL_Renderer to draw the image on.
 */
void WDG_drawImage(WDG_Image* image, SDL_Renderer* renderer);

/**
 * @brief Deletes an image widget and frees its resources.
 * @param image The WDG_Image object to be deleted.
 */
void WDG_deleteImage(WDG_Image* image);

#endif