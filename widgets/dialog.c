#include "dialog.h"
#include "init.h"
#include "colors.h"
#include <stdlib.h>

static void onSelectAnswer(WDG_Button* button, void* param) {
    WDG_Dialog* dialog = param;
    WDG_dialogSetAnswer(dialog, UTF8_copy(WDG_buttonGetText(button)));
}

static void onSelectButtonEnter(void* self, void* param) {
    WDG_Button* button = self;
    WDG_buttonSetBackgroundColor(button, WDG_BEIGE_DARK);
}

static void onSelectButtonLeave(void* self, void* param) {
    WDG_Button* button = self;
    WDG_buttonSetBackgroundColor(button, WDG_BEIGE);
}

static void validateOnClick(WDG_Button* button, void* param) {
    WDG_Dialog* dialog = param;
    WDG_dialogSetAnswer(dialog, UTF8_copy(dialog->input.textinput->text));
}

void WDG_dialogInit(WDG_Dialog* dialog, Character* character, int posx, int posy, int width, int height) {
    WDG_initWidget(&dialog->base, (SDL_Rect) {posx, posy, width, height}, (WDG_Padding) {0, 0, 0, 0});
    dialog->state = WDG_DIALOG_WAITING;
    dialog->answer = NULL;
    dialog->character = character;
    // common widgets
    WDG_TextArea* characterName = WDG_createTextArea(UTF8_copy(character->name), WDG_getFont(DEFAULT_FONT, 25), width, height);
    WDG_textareaSetPos(characterName, posx + 32, posy + 10);
    WDG_textareaSetFontColor(characterName, WDG_BLACK);
    dialog->characterName = characterName;
}

WDG_Dialog* WDG_createInputDialog(Character* character, UTF8_String* message, int posx, int posy, int width, int height) {
    WDG_Dialog* result = malloc(sizeof(WDG_Dialog));
    WDG_dialogInit(result, character, posx, posy, width, height);
    // dialog type
    result->kind = WDG_INPUT_DIALOG;
    // input dialog
    TTF_Font* commonFont = WDG_getFont(DEFAULT_FONT, 25);
    result->input.textarea = WDG_createTextArea(message, commonFont, width, height);
    result->input.textinput = WDG_createTextInput(commonFont, width * 2.0 / 3.0 - 20, 30);
    result->input.validate = WDG_createTextButton(UTF8_from("Valider"), commonFont, width * 1.0 / 3.0 - 30, 30);
    WDG_textareaSetPos(result->input.textarea, posx + 10, posy + 40);
    WDG_textinputSetPos(result->input.textinput, posx + 10, posy + height / 4);
    WDG_buttonSetPos(result->input.validate, posx + width * 2.0 / 3.0 + 20, posy + height / 4);
    // input
    WDG_textinputSetBorder(result->input.textinput, WDG_DOWN);
    WDG_textinputSetBorderColor(result->input.textinput, WDG_BEIGE_DARKER);
    WDG_textinputSetFontColor(result->input.textinput, WDG_BEIGE_DARKER);
    WDG_textinputSetBorderSize(result->input.textinput, 2);
    WDG_textinputSetFocus(result->input.textinput);
    WDG_textinputSetBackgroundColor(result->input.textinput, WDG_BEIGE_DARK);
    // button
    WDG_buttonSetFontColor(result->input.validate, WDG_BEIGE_DARKER);
    WDG_buttonSetBorderSize(result->input.validate, 3);
    WDG_buttonSetBackgroundColor(result->input.validate, WDG_BEIGE);
    WDG_buttonSetBorderColor(result->input.validate, WDG_BEIGE_DARKER);
    WDG_buttonSetOnMouseEnter(result->input.validate, onSelectButtonEnter, NULL);
    WDG_buttonSetOnMouseLeave(result->input.validate, onSelectButtonLeave, NULL);
    WDG_buttonSetOnClick(result->input.validate, validateOnClick, result);
    return result;
}

WDG_Dialog* WDG_createTextDialog(Character* character, UTF8_String* message, int posx, int posy, int width, int height) {
    WDG_Dialog* result = malloc(sizeof(WDG_Dialog));
    WDG_dialogInit(result, character, posx, posy, width, height);
    // dialog type
    result->kind = WDG_TEXT_DIALOG;
    // widgets definition
    TTF_Font* commonFont = WDG_getFont(DEFAULT_FONT, 25);
    result->text.textarea = WDG_createTextArea(message, commonFont, width, height);
    WDG_textareaSetPos(result->text.textarea, posx + 10, posy + 40);
    return result;
}

WDG_Dialog* WDG_createYesNoDialog(Character* character, UTF8_String* message, int posx, int posy, int width, int height) {
    WDG_Dialog* result = malloc(sizeof(WDG_Dialog));
    WDG_dialogInit(result, character, posx, posy, width, height);
    result->kind = WDG_YESNO_DIALOG;
    TTF_Font* commonFont = WDG_getFont(DEFAULT_FONT, 25);
    // construct the object
    result->yesNo.description = WDG_createTextArea(message, commonFont, width, height);
    WDG_textareaSetPos(result->yesNo.description, posx + 10, posy + 40);
    result->yesNo.yes = WDG_createTextButton(UTF8_from("Oui"), commonFont, width / 2 - 30, 30);
    WDG_buttonSetPos(result->yesNo.yes, posx + 10, posy + height / 4);
    WDG_buttonSetOnClick(result->yesNo.yes, onSelectAnswer, result);
    WDG_buttonSetBorderColor(result->yesNo.yes, WDG_BEIGE_DARKER);
    WDG_buttonSetFontColor(result->yesNo.yes, WDG_BEIGE_DARKER);
    WDG_buttonSetBorderSize(result->yesNo.yes, 3);
    WDG_buttonSetOnMouseEnter(result->yesNo.yes, onSelectButtonEnter, NULL);
    WDG_buttonSetOnMouseLeave(result->yesNo.yes, onSelectButtonLeave, NULL);
    result->yesNo.no = WDG_createTextButton(UTF8_from("Non"), commonFont, width / 2 - 30, 30);
    WDG_buttonSetPos(result->yesNo.no, posx + width / 2 + 10, posy + height / 4);
    WDG_buttonSetOnClick(result->yesNo.no, onSelectAnswer, result);
    WDG_buttonSetBorderColor(result->yesNo.no, WDG_BEIGE_DARKER);
    WDG_buttonSetFontColor(result->yesNo.no, WDG_BEIGE_DARKER);
    WDG_buttonSetBorderSize(result->yesNo.no, 3);
    WDG_buttonSetOnMouseEnter(result->yesNo.no, onSelectButtonEnter, NULL);
    WDG_buttonSetOnMouseLeave(result->yesNo.no, onSelectButtonLeave, NULL);
    return result;
}

WDG_Dialog* WDG_createChoicesDialog(Character* character, UTF8_String* message, ArrayList* answers, int posx, int posy, int width, int height) {
    WDG_Dialog* result = malloc(sizeof(WDG_Dialog));
    WDG_dialogInit(result, character, posx, posy, width, height);
    result->kind = WDG_CHOICE_DIALOG;
    TTF_Font* commonFont = WDG_getFont(DEFAULT_FONT, 25);
    // create the widgets
    result->choices.description = WDG_createTextArea(message, commonFont, width, height);
    WDG_textareaSetPos(result->choices.description, posx + 10, posy + 40);
    // create the choices
    result->choices.answers = ArrayList_new(WDG_deleteButton);
    for(size_t i = 0 ; i < ArrayList_length(answers) ; i += 1) {
        UTF8_String* buttonText = UTF8_copy(ArrayList_get(answers, i));
        WDG_Button* button = WDG_createTextButton(buttonText, commonFont, width / 2 - 30, 35);
        WDG_buttonSetPos(button, posx + 10 + (i % 2) * width / 2, posy + height / 4 + (i / 2) * 60);
        WDG_buttonSetOnClick(button, onSelectAnswer, result);
        WDG_buttonSetBorderColor(button, WDG_BEIGE_DARKER);
        WDG_buttonSetFontColor(button, WDG_BEIGE_DARKER);
        WDG_buttonSetBorderSize(button, 3);
        WDG_buttonSetOnMouseEnter(button, onSelectButtonEnter, NULL);
        WDG_buttonSetOnMouseLeave(button, onSelectButtonLeave, NULL);
        ArrayList_append(result->choices.answers, button);
    }
    return result;
}

void WDG_dialogSetAnswer(WDG_Dialog* dialog, UTF8_String* string) {
    dialog->state = WDG_DIALOG_ANSWER;
    dialog->answer = string;
}

void _WDG_drawInputDialog(WDG_Dialog* dialog, SDL_Renderer* renderer) {
    WDG_drawTextInput(dialog->input.textinput, renderer);
    WDG_drawTextArea(dialog->input.textarea, renderer);
    WDG_drawButton(dialog->input.validate, renderer);
}

void _WDG_drawTextDialog(WDG_Dialog* dialog, SDL_Renderer* renderer) {
    WDG_drawTextArea(dialog->text.textarea, renderer);
}

void _WDG_drawYesNoDialog(WDG_Dialog* dialog, SDL_Renderer* renderer) {
    WDG_drawTextArea(dialog->yesNo.description, renderer);
    WDG_drawButton(dialog->yesNo.yes, renderer);
    WDG_drawButton(dialog->yesNo.no, renderer);
}

void _WDG_drawChoicesDialog(WDG_Dialog* dialog, SDL_Renderer* renderer) {
    WDG_drawTextArea(dialog->choices.description, renderer);
    for(size_t i = 0 ; i < ArrayList_length(dialog->choices.answers) ; i += 1) {
        WDG_Button* button = ArrayList_get(dialog->choices.answers, i);
        WDG_drawButton(button, renderer);
    }
}

void WDG_dialogDraw(WDG_Dialog* dialog, SDL_Renderer* renderer) {
    if(dialog->kind != WDG_TEXT_DIALOG) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_RenderFillRect(renderer, NULL);
    }
    WDG_drawWidget(&dialog->base, renderer);
    WDG_drawTextArea(dialog->characterName, renderer);
    SDL_Rect src = {0, 0, 32, 32};
    SDL_Rect dst = {dialog->base.position.x, dialog->base.position.y + 10, 32, 32};
    SDL_RenderCopy(renderer, dialog->character->sdlPerso->sprite, &src, &dst);
    switch (dialog->kind) {
    case WDG_INPUT_DIALOG:
        _WDG_drawInputDialog(dialog, renderer);
        break;
    case WDG_TEXT_DIALOG:
        _WDG_drawTextDialog(dialog, renderer);
        break;
    case WDG_YESNO_DIALOG:
        _WDG_drawYesNoDialog(dialog, renderer);
        break;
    case WDG_CHOICE_DIALOG:
        _WDG_drawChoicesDialog(dialog, renderer);
        break;
    default:
        break;
    }
}

void _WDG_inputdialogDispatchEvent(WDG_Dialog* dialog, SDL_Event* event) {
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN) {
        UTF8_String* text = dialog->input.textinput->text;
        if(UTF8_length(text) != 0) {
            WDG_dialogSetAnswer(dialog, UTF8_copy(text)); 
        }
    }
    WDG_textinputDispatchEvent(dialog->input.textinput, event);
    WDG_buttonDispatchEvent(dialog->input.validate, event);
}

void _WDG_textdialogDispatchEvent(WDG_Dialog* dialog, SDL_Event* event) {
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN && dialog->state == WDG_DIALOG_WAITING) {
        WDG_dialogSetAnswer(dialog, UTF8_empty());
    }
}

void _WDG_yesNoDialogDispatchEvent(WDG_Dialog* dialog, SDL_Event* event) {
    WDG_buttonDispatchEvent(dialog->yesNo.yes, event);
    WDG_buttonDispatchEvent(dialog->yesNo.no, event);
}

void _WDG_choicesDialogDispatchEvent(WDG_Dialog* dialog, SDL_Event* event) {
    for(size_t i = 0 ; i < ArrayList_length(dialog->choices.answers) ; i += 1) {
        WDG_Button* button = ArrayList_get(dialog->choices.answers, i);
        WDG_buttonDispatchEvent(button, event);
    }
}

void WDG_dialogDispatchEvent(WDG_Dialog* dialog, SDL_Event* event) {
    if(WDG_widgetIsHidden(&dialog->base)) {
        return;
    }
    switch (dialog->kind) {
    case WDG_INPUT_DIALOG:
        _WDG_inputdialogDispatchEvent(dialog, event);
        break;
    case WDG_TEXT_DIALOG:
        _WDG_textdialogDispatchEvent(dialog, event);
        break;
    case WDG_YESNO_DIALOG:
        _WDG_yesNoDialogDispatchEvent(dialog, event);
        break;
    case WDG_CHOICE_DIALOG:
        _WDG_choicesDialogDispatchEvent(dialog, event);
        break;
    default:
        break;
    }
}

void _WDG_deleteInputDialog(WDG_Dialog* dialog) {
    WDG_deleteTextArea(dialog->input.textarea);
    WDG_deleteTextInput(dialog->input.textinput);
    WDG_deleteButton(dialog->input.validate);
}

void _WDG_deleteTextDialog(WDG_Dialog* dialog) {
    WDG_deleteTextArea(dialog->text.textarea);
}

void _WDG_deleteYesNoDialog(WDG_Dialog* dialog) {
    WDG_deleteTextArea(dialog->yesNo.description);
    WDG_deleteButton(dialog->yesNo.yes);
    WDG_deleteButton(dialog->yesNo.no);
}

void _WDG_deleteChoicesDialog(WDG_Dialog* dialog) {
    WDG_deleteTextArea(dialog->choices.description);
    ArrayList_delete(dialog->choices.answers);
}

void WDG_deleteDialog(WDG_Dialog* dialog) {
    switch (dialog->kind) {
    case WDG_INPUT_DIALOG:
        _WDG_deleteInputDialog(dialog);
        break;
    case WDG_TEXT_DIALOG:
        _WDG_deleteTextDialog(dialog);
        break;
    case WDG_YESNO_DIALOG:
        _WDG_deleteYesNoDialog(dialog);
        break;
    case WDG_CHOICE_DIALOG:
        _WDG_deleteChoicesDialog(dialog);
        break;
    default:
        break;
    }
    if(dialog->answer != NULL) {
        UTF8_delete(dialog->answer);
    }
    WDG_deleteTextArea(dialog->characterName);
    free(dialog);
}

void WDG_dialogAnimate(WDG_Dialog* dialog) {
    switch (dialog->kind) {
    case WDG_INPUT_DIALOG:
        WDG_animateTextArea(dialog->input.textarea);
        break;
    case WDG_TEXT_DIALOG:
        WDG_animateTextArea(dialog->text.textarea);
        break;
    case WDG_YESNO_DIALOG:
        WDG_animateTextArea(dialog->yesNo.description);
        break;
    case WDG_CHOICE_DIALOG:
        WDG_animateTextArea(dialog->choices.description);
        break;
    default:
        break;
    }
}