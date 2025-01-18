#ifndef WIDGETS_WIDGET_H
#define WIDGETS_WIDGET_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef void (*WDG_Callback)(void* caller, void* args);

typedef enum {
  WDG_NONE = 0,
  WDG_UP = 1,
  WDG_DOWN = 2,
  WDG_LEFT = 4,
  WDG_RIGHT = 8,
  WDG_ALL = 8 | 4 | 2 | 1
} WDG_Directions;

/**
 * @typedef WDG_WidgetState
 * @brief Enumeration for Widget state.
 *
 * This enum defines the different states a WDG_Widget can be in. Currently, it supports:
 *  - WDG_NORMAL: The default state of the widget.
 *  - WDG_MOUSEOVER: Indicates the mouse is hovering over the widget.
 */
typedef enum {
  WDG_NORMAL = 1,
  WDG_MOUSEOVER = 2,
  WDG_FOCUSED = 4,
  WDG_HIDDEN = 8
} WDG_WidgetState;

/**
 * @typedef WDG_Padding
 * @brief Structure representing widget padding.
 *
 * This structure defines the padding around the content area of a widget.
 *  - top: Padding from the top edge.
 *  - down: Padding from the bottom edge.
 *  - left: Padding from the left edge.
 *  - right: Padding from the right edge.
 */
typedef struct {
  int top;
  int down;
  int left;
  int right;
} WDG_Padding;

typedef struct {
  WDG_Directions borders;
  SDL_Color borderColor;
  int borderSize;
} WDG_Borders;

/**
 * @brief Structure representing a WDG Widget.
 */
typedef struct WDG_Widget {
  SDL_Rect position;  // Defines the widget's position on the screen (x, y, width, height).
  WDG_Padding padding;  // Padding around the widget's content area.
  WDG_WidgetState state;  // Current state of the widget (WDG_NORMAL, WDG_MOUSEOVER, etc.).
  SDL_Texture* texture;  // Optional texture associated with the widget for rendering visuals.
  WDG_Borders borders;
  SDL_Color backgroundColor;
  struct {
    WDG_Callback mouseEnter;
    WDG_Callback mouseLeave;
    void* mouseEnterArgs;
    void* mouseLeaveArgs;
  } callbacks;
} WDG_Widget;

// Function prototypes with comments
/**
 * @brief Initializes a WDG_Widget object.
 * @param widget The WDG_Widget object to be initialized.
 * @param position The SDL_Rect defining the widget's initial position on the screen.
 * @param padding The WDG_Padding structure defining the padding around the widget's content area.
 */
void WDG_initWidget(WDG_Widget* widget, SDL_Rect position, WDG_Padding padding);

/**
 * @brief Initializes a WDG_Borders object.
 * @param borders The WDG_Borders object to be initialized.
 * @param where The direction where the border is to be set.
 * @param color The color of the border.
 * @param borderSize The size of the border.
 */
void WDG_initBorder(WDG_Borders* borders, WDG_Directions where, SDL_Color color, int borderSize);

/**
 * @brief Sets the position of a WDG_Widget on the screen.
 * @param widget The WDG_Widget object to modify.
 * @param x The new x-coordinate of the top-left corner of the widget.
 * @param y The new y-coordinate of the top-left corner of the widget.
 */
void WDG_widgetSetPos(WDG_Widget* widget, int x, int y);

/**
 * @brief Sets the callback function to be executed when the mouse enters the widget.
 * @param widget The WDG_Widget object to modify.
 * @param callback The callback function to be executed.
 * @param args Additional arguments to be passed to the callback function.
 */
void WDG_widgetSetOnMouseEnter(WDG_Widget* widget, WDG_Callback callback, void* args);

/**
 * @brief Sets the callback function to be executed when the mouse leaves the widget.
 * @param widget The WDG_Widget object to modify.
 * @param callback The callback function to be executed.
 * @param args Additional arguments to be passed to the callback function.
 */
void WDG_widgetSetOnMouseLeave(WDG_Widget* widget, WDG_Callback callback, void* args);

/**
 * @brief Sets the state of a WDG_Widget.
 * @param widget The WDG_Widget object to modify.
 * @param state The state to set.
 */
void WDG_widgetSetState(WDG_Widget* widget, WDG_WidgetState state);

/**
 * @brief Toggles the state of a WDG_Widget.
 * @param widget The WDG_Widget object to modify.
 * @param state The state to toggle.
 */
void WDG_widgetToggleState(WDG_Widget* widget, WDG_WidgetState state);

/**
 * @brief Checks if a WDG_Widget has a specific state.
 * @param widget The WDG_Widget object to check.
 * @param state The state to check.
 * @return true if the widget has the specified state, otherwise false.
 */
bool WDG_widgetIs(WDG_Widget* widget, WDG_WidgetState state);

/**
 * @brief Checks if a WDG_Widget is focused.
 * @param widget The WDG_Widget object to check.
 * @return true if the widget is focused, otherwise false.
 */
bool WDG_widgetIsFocused(WDG_Widget* widget);

/**
 * @brief Checks if a WDG_Widget is hidden.
 * @param widget The WDG_Widget object to check.
 * @return true if the widget is hidden, otherwise false.
 */
bool WDG_widgetIsHidden(WDG_Widget* widget);

/**
 * @brief Checks if the mouse cursor is hovering over a WDG_Widget.
 * @param widget The WDG_Widget object to check.
 * @return true if the mouse cursor is hovering over the widget, otherwise false.
 */
bool WDG_widgetIsHovered(WDG_Widget* widget);

/**
 * @brief Sets focus to a WDG_Widget.
 * @param widget The WDG_Widget object to set focus on.
 */
void WDG_widgetSetFocus(WDG_Widget* widget);

/**
 * @brief Sets the border of a WDG_Widget.
 * @param widget The WDG_Widget object to modify.
 * @param border The direction(s) where the border is to be set.
 */
void WDG_widgetSetBorder(WDG_Widget* widget, WDG_Directions border);

/**
 * @brief Sets the color of the border of a WDG_Widget.
 * @param widget The WDG_Widget object to modify.
 * @param color The color of the border.
 */
void WDG_widgetSetBorderColor(WDG_Widget* widget, SDL_Color color);

/**
 * @brief Sets the size of the border of a WDG_Widget.
 * @param widget The WDG_Widget object to modify.
 * @param borderSize The size of the border.
 */
void WDG_widgetSetBorderSize(WDG_Widget* widget, int borderSize);

/**
 * @brief Sets the background color of a WDG_Widget.
 * @param widget The WDG_Widget object to modify.
 * @param color The color of the background.
 */
void WDG_widgetSetBackgroundColor(WDG_Widget* widget, SDL_Color color);

/**
 * @brief Gets the outer rectangle of a WDG_Widget (including padding).
 * @param widget The WDG_Widget object to query.
 * @return An SDL_Rect representing the outer rectangle of the widget.
 */
SDL_Rect WDG_WidgetGetOuterRect(const WDG_Widget* widget);

/**
 * @brief Gets the inner rectangle of a WDG_Widget (excluding padding).
 * @param widget The WDG_Widget object to query.
 * @return An SDL_Rect representing the inner rectangle of the widget.
 */

SDL_Rect WDG_WidgetGetInnerRect(const WDG_Widget* widget);
/**
 * @brief Gets the current state of a WDG_Widget.
 * @param widget The WDG_Widget object to query.
 * @return The current state of the widget (WDG_WidgetState).
 */
WDG_WidgetState WDG_widgetGetState(const WDG_Widget* widget);

/**
 * @brief Checks for collision between a point and a WDG_Widget.
 * @param widget The WDG_Widget object to check for collision.
 * @param x The x-coordinate of the point to check.
 * @param y The y-coordinate of the point to check.
 * @return True if the point collides with the widget, False otherwise.
 */
bool WDG_widgetCollide(const WDG_Widget* widget, int x, int y);

void WDG_widgetHandleEvent(WDG_Widget* widget, SDL_Event* event);

void WDG_drawWidget(WDG_Widget* widget, SDL_Renderer* renderer);

#endif