#ifndef WIDGETS_TEXTINPUT_H
#define WIDGETS_TEXTINPUT_H

#include "widget.h"
#include "utils.h"
#include "../utf8strings/utf8strings.h"
#include <stddef.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <limits.h>


#define WDG_TEXTINPUT_PADDING 5
#define WDG_TEXTINPUT_MAXLENGTH (SIZE_MAX / 4 - 1)
#define WDG_TEXTINPUT_CURSOR_SIZE 15

#define WDG_textinputSetState(widget, state) WDG_widgetSetState((WDG_Widget*)widget, state)
#define WDG_textinputToggleState(widget, state) WDG_widgetToggleState((WDG_Widget*)widget, state)
#define WDG_textinputIs(widget, state) WDG_widgetIs((WDG_Widget*)widget, state)
#define WDG_textinputIsFocused(widget) WDG_widgetIsFocused((WDG_Widget*)widget)
#define WDG_textinputIsHidden(widget) WDG_widgetIsHidden((WDG_Widget*)widget)
#define WDG_textinputIsHovered(widget) WDG_widgetIsHovered((WDG_Widget*)widget)
#define WDG_textinputSetFocus(widget) WDG_widgetSetFocus((WDG_Widget*)widget)
#define WDG_textinputSetBorder(widget, border) WDG_widgetSetBorder((WDG_Widget*)widget, border)
#define WDG_textinputSetBorderColor(widget, color) WDG_widgetSetBorderColor((WDG_Widget*)widget, color)
#define WDG_textinputSetBorderSize(widget, borderSize) WDG_widgetSetBorderSize((WDG_Widget*)widget, borderSize)
#define WDG_textinputSetBackgroundColor(widget, backgroundColor) WDG_widgetSetBackgroundColor((WDG_Widget*)widget, backgroundColor)


/**
 * @brief Structure representing a WDG Text Input widget.
 *
 * This structure defines the properties of a WDG_TextInput widget, which is a single-line text input field
 * that allows users to enter and edit text. It inherits properties from a base WDG_Widget.
 */
typedef struct {
  WDG_Widget base;
  /**
   * @brief Maximum allowed length of the text input.
   */
  size_t maxLength;
  /**
   * @brief Text currently displayed in the text input field.
   */
  UTF8_String* text;
  /**
   * @brief Font used to render the text input text.
   */
  TTF_Font* textFont;
  WDG_Color fontColor;
} WDG_TextInput;

// Function prototypes with comments
/**
 * @brief Creates a new WDG_TextInput object.
 * @param textFont The TTF_Font object to be used for rendering the text input text.
 * @param width The desired width of the text input field in pixels.
 * @param height The desired height of the text input field in pixels.
 * @return A pointer to the newly created WDG_TextInput object, or NULL on failure.
 */
WDG_TextInput* WDG_createTextInput(TTF_Font *textFont, int width, int height);

/**
 * @brief Sets the maximum allowed length of the text input field.
 * @param textinput The WDG_TextInput object to modify.
 * @param maxLength The new maximum length for the text input field.
 */
void WDG_textinputSetMaxLength(WDG_TextInput* textinput, size_t maxLength);

/**
 * @brief Sets the position of a WDG_TextInput on the screen.
 * @param textinput The WDG_TextInput object to modify.
 * @param x The x-coordinate of the top-left corner of the text input field.
 * @param y The y-coordinate of the top-left corner of the text input field.
 */
void WDG_textinputSetPos(WDG_TextInput* textinput, int x, int y);

void WDG_textinputSetFontColor(WDG_TextInput* textinput, WDG_Color color);

/**
 * @brief Deletes a text input object.
 * @param textinput The text input object to delete.
 */
void WDG_deleteTextInput(WDG_TextInput* textinput);

/**
 * @brief Draws a text input object to the screen.
 * @param textinput The text input object to draw.
 * @param renderer The SDL renderer to use for drawing.
 */
void WDG_drawTextInput(WDG_TextInput* textinput, SDL_Renderer* renderer);

/**
 * @brief Appends text to a text input object.
 * @param textinput The text input object to modify.
 * @param str The text to append.
 */
void WDG_textinputEventTextAppend(WDG_TextInput* textinput, const char* str);

/**
 * @brief Erases text from a text input object.
 * @param textinput The text input object to modify.
 */
void WDG_textinputEventErase(WDG_TextInput* textinput);

/**
 * @brief Dispatches an SDL event to a text input object.
 * @param textinput The text input object to receive the event.
 * @param event The SDL event to dispatch.
 * @remarks This function handles events that are relevant to text input, such as
 *          key presses and mouse clicks. It updates the text input's state and
 *          appearance based on the event.
 */
void WDG_textinputDispatchEvent(WDG_TextInput* textinput, SDL_Event* event);

#endif