#ifndef WIDGETS_TEXTAREA_H
#define WIDGETS_TEXTAREA_H

#include "widget.h"
#include "utils.h"
#include "../utf8strings/utf8strings.h"
#include "../animations/animation.h"
#include "../animations/animationmanager.h"
#include <stddef.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>


#define WDG_TEXTAREA_PADDING 5

#define WDG_textareaSetState(widget, state) WDG_widgetSetState((WDG_Widget*)widget, state)
#define WDG_textareaToggleState(widget, state) WDG_widgetToggleState((WDG_Widget*)widget, state)
#define WDG_textareaIs(widget, state) WDG_widgetIs((WDG_Widget*)widget, state)
#define WDG_textareaIsFocused(widget) WDG_widgetIsFocused((WDG_Widget*)widget)
#define WDG_textareaIsHidden(widget) WDG_widgetIsHidden((WDG_Widget*)widget)
#define WDG_textareaIsHovered(widget) WDG_widgetIsHovered((WDG_Widget*)widget)
#define WDG_textareaSetBorder(widget, border) WDG_widgetSetBorder((WDG_Widget*)widget, border)
#define WDG_textareaSetBorderColor(widget, color) WDG_widgetSetBorderColor((WDG_Widget*)widget, color)
#define WDG_textareaSetBorderSize(widget, borderSize) WDG_widgetSetBorderSize((WDG_Widget*)widget, borderSize)
#define WDG_textareaSetBackgroundColor(widget, backgroundColor) WDG_widgetSetBackgroundColor((WDG_Widget*)widget, backgroundColor)


/**
* @brief Structure representing a static textarea widget.
*/
typedef struct {
    WDG_Widget base;
    /**
     * @brief Text displayed in the text area.
     */
    UTF8_String* text;
    /**
     * @brief Font used to render the text area text.
     */
    TTF_Font* textFont;
    WDG_Color fontColor;
    Animation* animation;
} WDG_TextArea;

/**
* @brief Creates a new WDG_TextArea object.
* @param text The UTF8_String representing the initial text to be displayed in the text area.
* @param textFont The TTF_Font object to be used for rendering the text area text.
* @param width The desired width of the text area in pixels.
* @param height The desired height of the text area in pixels.
*
* @return A pointer to the newly created WDG_TextArea object, or NULL on failure.
*/
WDG_TextArea* WDG_createTextArea(UTF8_String* text, TTF_Font* textFont, int width, int height);

/**
 * @brief Sets the position of a WDG_TextArea on the screen.
 * @param textarea The WDG_TextArea object to modify.
 * @param x The x-coordinate of the top-left corner of the text area.
 * @param y The y-coordinate of the top-left corner of the text area.
 */
void WDG_textareaSetPos(WDG_TextArea* textarea, int x, int y);

/**
 * @brief Sets the font color of a WDG_TextArea.
 * @param textarea The WDG_TextArea object to modify.
 * @param color The color to set.
 */
void WDG_textareaSetFontColor(WDG_TextArea* textarea, WDG_Color color);

/**
 * @brief Sets the text of a WDG_TextArea.
 * @param textarea The WDG_TextArea object to modify.
 * @param string The text to set.
 */
void WDG_textareaSetText(WDG_TextArea* textarea, UTF8_String* string);

/**
 * @brief Deletes a WDG_TextArea object and frees its resources.
 * @param textarea The WDG_TextArea object to be deleted.
 */
void WDG_deleteTextArea(WDG_TextArea* textarea);

/**
 * @brief Draws the WDG_TextArea on the specified SDL_Renderer.
 * @param textarea The WDG_TextArea object to be drawn.
 * @param renderer The SDL_Renderer to draw the text area on.
 */
void WDG_drawTextArea(WDG_TextArea* textarea, SDL_Renderer* renderer);

/**
 * @brief Animates the WDG_TextArea.
 * @param textarea The WDG_TextArea object to be animated.
 */
void WDG_animateTextArea(WDG_TextArea* textarea);

#endif