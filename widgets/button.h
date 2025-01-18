#ifndef WIDGETS_BUTTON_H
#define WIDGETS_BUTTON_H

#include "widget.h"
#include "utils.h"
#include "image.h"
#include "textarea.h"
#include "../utf8strings/utf8strings.h"
#include <stddef.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>


#define WDG_BUTTON_PADDING 5

#define WDG_buttonSetState(widget, state) WDG_widgetSetState((WDG_Widget*)widget, state)
#define WDG_buttonToggleState(widget, state) WDG_widgetToggleState((WDG_Widget*)widget, state)
#define WDG_buttonIs(widget, state) WDG_widgetIs((WDG_Widget*)widget, state)
#define WDG_buttonIsFocused(widget) WDG_widgetIsFocused((WDG_Widget*)widget)
#define WDG_buttonIsHidden(widget) WDG_widgetIsHidden((WDG_Widget*)widget)
#define WDG_buttonIsHovered(widget) WDG_widgetIsHovered((WDG_Widget*)widget)
#define WDG_buttonSetBorder(widget, border) WDG_widgetSetBorder((WDG_Widget*)widget, border)
#define WDG_buttonSetBorderColor(widget, color) WDG_widgetSetBorderColor((WDG_Widget*)widget, color)
#define WDG_buttonSetBorderSize(widget, borderSize) WDG_widgetSetBorderSize((WDG_Widget*)widget, borderSize)
#define WDG_buttonSetBackgroundColor(widget, backgroundColor) WDG_widgetSetBackgroundColor((WDG_Widget*)widget, backgroundColor)
#define WDG_buttonSetOnMouseEnter(widget, callback, args) WDG_widgetSetOnMouseEnter((WDG_Widget*) widget, callback, args)
#define WDG_buttonSetOnMouseLeave(widget, callback, args) WDG_widgetSetOnMouseLeave((WDG_Widget*) widget, callback, args)


/**
 * @brief Structure representing a button widget.
 */
typedef struct _WDG_Button {
  WDG_Widget base;
  /**
   * @brief Text displayed on the button.
   */
  WDG_TextArea* text;
  WDG_Image* image;
  /**
   * @brief Callback function for button interactions.
   */
  void (*callback)(struct _WDG_Button* button, void* args);
  void* callbackArgs;
} WDG_Button;


/**
 * @typedef WDG_ButtonCallback
 * @brief Callback function type for WDG_Buttons.
 */
typedef void (*WDG_ButtonCallback)(WDG_Button*, void* args);

// Function prototypes with comments
/**
 * @brief Creates a new WDG_Button object with text.
 * @param text The UTF8_String representing the text to be displayed on the button.
 * @param textFont The TTF_Font object to be used for rendering the button text.
 * @param width The width of the button.
 * @param height The height of the button.
 * @return A pointer to the newly created WDG_Button object, or NULL on failure.
 */
WDG_Button* WDG_createTextButton(UTF8_String* text, TTF_Font* textFont, int width, int height);

/**
 * @brief Creates a new WDG_Button object with an image.
 * @param image The WDG_Image object representing the image to be displayed on the button.
 * @param width The width of the button.
 * @param height The height of the button.
 * @return A pointer to the newly created WDG_Button object, or NULL on failure.
 */
WDG_Button* WDG_createImageButton(WDG_Image* image, int width, int height);

/**
 * @brief Sets the position of a WDG_Button on the screen.
 * @param button The WDG_Button object to modify.
 * @param x The x-coordinate of the top-left corner of the button.
 * @param y The y-coordinate of the top-left corner of the button.
 */
void WDG_buttonSetPos(WDG_Button* button, int x, int y);

/**
 * @brief Sets the callback function to be executed when the button is clicked.
 * @param button The WDG_Button object to modify.
 * @param callback The callback function to be executed.
 * @param args Additional arguments to be passed to the callback function.
 */
void WDG_buttonSetOnClick(WDG_Button* button, WDG_ButtonCallback callback, void* args);

/**
 * @brief Sets the font color of the text on the button.
 * @param button The WDG_Button object to modify.
 * @param color The color to set.
 */
void WDG_buttonSetFontColor(WDG_Button* button, WDG_Color color);

/**
 * @brief Retrieves the text of the button.
 * @param button The WDG_Button object.
 * @return The text of the button.
 */
UTF8_String* WDG_buttonGetText(WDG_Button* button);

/**
 * @brief Deletes a WDG_Button object and frees its resources.
 * @param button The WDG_Button object to be deleted.
 */
void WDG_deleteButton(WDG_Button* button);

/**
 * @brief Draws the WDG_Button on the specified SDL_Renderer.
 * @param button The WDG_Button object to be drawn.
 * @param renderer The SDL_Renderer to draw the button on.
 */
void WDG_drawButton(WDG_Button* button, SDL_Renderer* renderer);

/**
 * @brief Retrieves the base WDG_Widget of a WDG_Button.
 * @param button The WDG_Button object to get the base widget from.
 * @return A pointer to the base WDG_Widget of the button.
 */
WDG_Widget* WDG_buttonGetBase(WDG_Button* button);

/**
 * @brief Dispatches an SDL event to the button for event handling.
 * @param button The WDG_Button object to handle the event.
 * @param event The SDL_Event to be handled.
 */
void WDG_buttonDispatchEvent(WDG_Button* button, SDL_Event* event);

#endif