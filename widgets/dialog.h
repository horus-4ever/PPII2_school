#ifndef WDG_DIALOG_H
#define WDG_DIALOG_H

#include "textarea.h"
#include "button.h"
#include "textinput.h"
#include "../quest/character.h"
#include "../animations/animationmanager.h"

#define WDG_dialogSetBorder(widget, border) WDG_widgetSetBorder((WDG_Widget*)widget, border)
#define WDG_dialogSetBorderColor(widget, color) WDG_widgetSetBorderColor((WDG_Widget*)widget, color)
#define WDG_dialogSetBorderSize(widget, borderSize) WDG_widgetSetBorderSize((WDG_Widget*)widget, borderSize)
#define WDG_dialogSetBackgroundColor(widget, backgroundColor) WDG_widgetSetBackgroundColor((WDG_Widget*)widget, backgroundColor)

/**
 * @brief Enumerates the states of a WDG_Dialog.
 */
typedef enum {
    WDG_DIALOG_WAITING, ///< The dialog is waiting for user interaction.
    WDG_DIALOG_ANSWER    ///< The dialog is in a state where an answer is expected.
} WDG_DialogState;

/**
 * @brief Enumerates the kinds of dialogs.
 */
typedef enum {
    WDG_INPUT_DIALOG,   ///< Input dialog where the user can input text.
    WDG_YESNO_DIALOG,   ///< Yes/No dialog where the user can choose between two options.
    WDG_CHOICE_DIALOG,  ///< Choice dialog where the user can select from multiple options.
    WDG_TEXT_DIALOG     ///< Text dialog for displaying information.
} WDG_DialogKind;

/**
 * @brief Represents a dialog widget.
 */
typedef struct {
    WDG_Widget base;             ///< The base widget.
    WDG_DialogState state;       ///< The state of the dialog.
    WDG_DialogKind kind;         ///< The kind of dialog.
    union {
        struct {
            WDG_TextArea* textarea;    ///< The text area for displaying the message.
            WDG_TextInput* textinput;  ///< The text input for user input.
            WDG_Button* validate;      ///< The validation button.
        } input;                       ///< Input dialog specific components.
        struct {
            WDG_TextArea* textarea;    ///< The text area for displaying the message.
        } text;                        ///< Text dialog specific components.
        struct {
            WDG_TextArea* description; ///< The text area for displaying the message.
            WDG_Button* yes;           ///< The "Yes" button.
            WDG_Button* no;            ///< The "No" button.
        } yesNo;                       ///< Yes/No dialog specific components.
        struct {
            WDG_TextArea* description; ///< The text area for displaying the message.
            ArrayList* answers;        ///< The list of choices.
        } choices;                     ///< Choice dialog specific components.
    };
    Character* character;          ///< The character associated with the dialog.
    WDG_TextArea* characterName;   ///< The text area for displaying the character's name.
    UTF8_String* answer;           ///< The user's answer.
} WDG_Dialog;

/**
 * @brief Initializes a WDG_Dialog object.
 * @param dialog The WDG_Dialog object to be initialized.
 * @param character The character associated with the dialog.
 * @param posx The x-coordinate of the dialog's position.
 * @param posy The y-coordinate of the dialog's position.
 * @param width The width of the dialog.
 * @param height The height of the dialog.
 */
void WDG_dialogInit(WDG_Dialog* dialog, Character* character, int posx, int posy, int width, int height);

/**
 * @brief Creates a new input dialog.
 * @param character The character associated with the dialog.
 * @param message The message to be displayed in the dialog.
 * @param posx The x-coordinate of the dialog's position.
 * @param posy The y-coordinate of the dialog's position.
 * @param width The width of the dialog.
 * @param height The height of the dialog.
 * @return A pointer to the newly created input dialog.
 */
WDG_Dialog* WDG_createInputDialog(Character* character, UTF8_String* message, int posx, int posy, int width, int height);

/**
 * @brief Creates a new text dialog.
 * @param character The character associated with the dialog.
 * @param message The message to be displayed in the dialog.
 * @param posx The x-coordinate of the dialog's position.
 * @param posy The y-coordinate of the dialog's position.
 * @param width The width of the dialog.
 * @param height The height of the dialog.
 * @return A pointer to the newly created text dialog.
 */
WDG_Dialog* WDG_createTextDialog(Character* character, UTF8_String* message, int posx, int posy, int width, int height);

/**
 * @brief Creates a new yes/no dialog.
 * @param character The character associated with the dialog.
 * @param message The message to be displayed in the dialog.
 * @param posx The x-coordinate of the dialog's position.
 * @param posy The y-coordinate of the dialog's position.
 * @param width The width of the dialog.
 * @param height The height of the dialog.
 * @return A pointer to the newly created yes/no dialog.
 */
WDG_Dialog* WDG_createYesNoDialog(Character* character, UTF8_String* message, int posx, int posy, int width, int height);

/**
 * @brief Creates a new choice dialog.
 * @param character The character associated with the dialog.
 * @param message The message to be displayed in the dialog.
 * @param answers The list of choices.
 * @param posx The x-coordinate of the dialog's position.
 * @param posy The y-coordinate of the dialog's position.
 * @param width The width of the dialog.
 * @param height The height of the dialog.
 * @return A pointer to the newly created choice dialog.
 */
WDG_Dialog* WDG_createChoicesDialog(Character* character, UTF8_String* message, ArrayList* answers, int posx, int posy, int width, int height);

/**
 * @brief Sets the user's answer for the dialog.
 * @param dialog The WDG_Dialog object.
 * @param string The user's answer.
 */
void WDG_dialogSetAnswer(WDG_Dialog* dialog, UTF8_String* string);

/**
 * @brief Draws the WDG_Dialog on the specified SDL_Renderer.
 * @param dialog The WDG_Dialog object to be drawn.
 * @param renderer The SDL_Renderer to draw the dialog on.
 */
void WDG_dialogDraw(WDG_Dialog* dialog, SDL_Renderer* renderer);

/**
 * @brief Dispatches an SDL event to the dialog for event handling.
 * @param dialog The WDG_Dialog object.
 * @param event The SDL_Event to be dispatched.
 */
void WDG_dialogDispatchEvent(WDG_Dialog* dialog, SDL_Event* event);

/**
 * @brief Deletes a WDG_Dialog object and frees its resources.
 * @param dialog The WDG_Dialog object to be deleted.
 */
void WDG_deleteDialog(WDG_Dialog* dialog);

/**
 * @brief Animates the WDG_Dialog.
 * @param dialog The WDG_Dialog object to be animated.
 */
void WDG_dialogAnimate(WDG_Dialog* dialog);

#endif