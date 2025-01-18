#include "interpreter.h"
#include "tnobject/objects.h"
#include "widgets/widget.h"
#include "fonctions_SDL.h"
#include "datastructures/stack.h"
#include "utf8strings/utf8strings.h"
#include "animations/itemanimation.h"
#include "animations/questanimation.h"
#include "animations/typewriteranimation.h"
#include "sound.h"
#include "npc.h"
#include "game.h"
#include <stdlib.h>

Interpreter* Interpreter_new(Character* player) {
    // create the player object for the interpreter
    Tn_Object* mpc = Tn_Object_newCharacter(player);
    Tn_Object_INCREF(mpc);
    // create the interpreter
    Interpreter* result = malloc(sizeof(Interpreter));
    result->character = NULL;
    result->bytecode = NULL;
    result->dialog = NULL;
    result->globals = NULL;
    result->animation = NULL;
    result->state = INTERPRETER_STOPPED;
    result->player = mpc;
    result->stack = Stack_new(Tn_Object_DECREF);
    result->scope = HashMap_new(UTF8_delete, Tn_Object_DECREF, UTF8_equal, UTF8_hash);
    Interpreter_reset(result);
    return result;
}

void Interpreter_setGame(Interpreter* interpreter, Game* game) {
    interpreter->game = game;
}

void Interpreter_reset(Interpreter* interpreter) {
    if(interpreter->dialog != NULL) {
        WDG_deleteDialog(interpreter->dialog);
        interpreter->dialog = NULL;
    }
    interpreter->IP = 0;
    interpreter->state = INTERPRETER_STOPPED;
}

void Interpreter_setGlobals(Interpreter* interpreter, HashMap* globals) {
    if(interpreter->globals != NULL) {
        HashMap_delete(interpreter->globals);
    }
    Tn_Object_INCREF(interpreter->player);
    HashMap_set(globals, UTF8_from("player"), interpreter->player);
    interpreter->globals = globals;
}

void STACKPUSH(Interpreter* interpreter, Tn_Object* object) {
    Tn_Object_INCREF(object);
    Stack_push(interpreter->stack, object);
}

Tn_Object* STACKPOP(Interpreter* interpreter) {
    Tn_Object* result = Stack_pop(interpreter->stack);
    return result;
}

Tn_Object* GETVARIABLE(Interpreter* interpreter, size_t number) {
    UTF8_String* varname = ArrayList_get(interpreter->bytecode->varnames, number);
    Tn_Object* object = HashMap_get(interpreter->scope, varname);
    if(object == NULL && interpreter->globals != NULL) {
        object = HashMap_get(interpreter->globals, varname);
        if(object == NULL) {
            printf("Interpreter error\n-> Unknown variable '%s'\n", varname->data);
            exit(-1);
        }
    }
    return object;
}

void SETVARIABLE(Interpreter* interpreter, Tn_Object* object, size_t number) {
    UTF8_String* varname = ArrayList_get(interpreter->bytecode->varnames, number);
    HashMap_set(interpreter->scope, UTF8_copy(varname), object);
}

UTF8_String* GETSTRING(Interpreter* interpreter, size_t number) {
    return ArrayList_get(interpreter->bytecode->strings, number);
}

void Interpreter_loadAndRun(Interpreter* interpreter, Bytecode* bytecode) {
    Interpreter_reset(interpreter);
    interpreter->bytecode = bytecode;
    interpreter->state = INTERPRETER_RUNNING;
    Interpreter_tick(interpreter);
}

void _Interpreter_setDialog(Interpreter* interpreter, WDG_Dialog* dialog) {
    if(interpreter->dialog != NULL) {
        WDG_deleteDialog(interpreter->dialog);
    }
    interpreter->dialog = dialog;
}

void _executeSayLine(Interpreter* interpreter) {
    Tn_Object* text = STACKPOP(interpreter);
    Tn_Object* character = STACKPOP(interpreter);
    WDG_Dialog* dialog = WDG_createTextDialog(character->character, UTF8_copy(text->string), 0, HEIGHT - 100, WIDTH, 100);
    WDG_dialogSetBackgroundColor(dialog, WDG_BEIGE);
    _Interpreter_setDialog(interpreter, dialog);
    // textanimation
    WDG_dialogAnimate(dialog);
    // wait for input
    Interpreter_waitInput(interpreter);
    // decrease refcount
    Tn_Object_DECREF(text);
    Tn_Object_DECREF(character);
}

void _executeAsk(Interpreter* interpreter) {
    Tn_Object* text = STACKPOP(interpreter);
    Character* current = interpreter->bytecode->contextObject->character;
    WDG_Dialog* dialog = WDG_createInputDialog(current, UTF8_copy(text->string), WIDTH / 2 - WIDTH / 4, HEIGHT / 2 - HEIGHT / 4, WIDTH / 2, HEIGHT / 2);
    WDG_dialogSetBackgroundColor(dialog, WDG_BEIGE);
    WDG_dialogSetBorder(dialog, WDG_ALL);
    WDG_dialogSetBorderColor(dialog, WDG_BEIGE_DARKER);
    WDG_dialogSetBorderSize(dialog, 5);
    _Interpreter_setDialog(interpreter, dialog);
    // text animation
    WDG_dialogAnimate(dialog);
    // wait for input
    Interpreter_waitInput(interpreter);
    // decrease refcount
    Tn_Object_DECREF(text);
}

void _executeAskYesNo(Interpreter* interpreter) {
    Tn_Object* text = STACKPOP(interpreter);
    Character* current = interpreter->bytecode->contextObject->character;
    WDG_Dialog* dialog = WDG_createYesNoDialog(current, UTF8_copy(text->string), WIDTH / 2 - WIDTH / 4, HEIGHT / 2 - HEIGHT / 4, WIDTH / 2, HEIGHT / 2);
    WDG_dialogSetBackgroundColor(dialog, WDG_BEIGE);
    WDG_dialogSetBorder(dialog, WDG_ALL);
    WDG_dialogSetBorderColor(dialog, WDG_BEIGE_DARKER);
    WDG_dialogSetBorderSize(dialog, 5);
    _Interpreter_setDialog(interpreter, dialog);
    // text animation
    WDG_dialogAnimate(dialog);
    // wait for input
    Interpreter_waitInput(interpreter);
    // decrease refcount
    Tn_Object_DECREF(text);
}

void _executeAskChoices(Interpreter* interpreter) {
    Tn_Object* number = STACKPOP(interpreter);
    ArrayList* choices = ArrayList_new(UTF8_delete);
    for(size_t i = 0 ; i < (size_t) number->number ; i += 1) {
        Tn_Object* choice = STACKPOP(interpreter);
        ArrayList_append(choices, UTF8_copy(choice->string));
        Tn_Object_DECREF(choice);
    }
    Tn_Object* text = STACKPOP(interpreter);
    Character* current = interpreter->bytecode->contextObject->character;
    WDG_Dialog* dialog = WDG_createChoicesDialog(current, UTF8_copy(text->string), choices, WIDTH / 2 - WIDTH / 4, HEIGHT / 2 - HEIGHT / 4, WIDTH / 2, HEIGHT / 2);
    WDG_dialogSetBackgroundColor(dialog, WDG_BEIGE);
    WDG_dialogSetBorder(dialog, WDG_ALL);
    WDG_dialogSetBorderColor(dialog, WDG_BEIGE_DARKER);
    WDG_dialogSetBorderSize(dialog, 5);
    _Interpreter_setDialog(interpreter, dialog);
    // text animation
    WDG_dialogAnimate(dialog);
    // wait for input
    Interpreter_waitInput(interpreter);
    // decrease refcount
    ArrayList_delete(choices);
    Tn_Object_DECREF(number);
    Tn_Object_DECREF(text);
}

void _executeGive(Interpreter* interpreter) {
    Tn_Object* text = STACKPOP(interpreter);
    // the player receives the object
    Quest* quest = QuestManager_getCurrentQuest(interpreter->game->questManager);
    Quest_findItem(quest, text->string);
    // launch the animation
    int index = index_of_item(quest->items, text->string);
    SDL_Texture* texture = quest->items->sprite[index];
    ItemAnimationContext* context = ItemAnimationContext_new(texture);
    Animation* animation = ItemAnimation_new(context);
    Interpreter_launchAnimation(interpreter, animation, true);
    play_sound(interpreter->game->soundManager->sounds, INDEX_ITEM_RECUP);
    // interpreter->state = INTERPRETER_WAITING;
    STACKPUSH(interpreter, text);
    // decrease refcount
    Tn_Object_DECREF(text);
}

void _executeGotoSubquest(Interpreter* interpreter) {
    Tn_Object* obj = STACKPOP(interpreter);
    size_t number = obj->number;
    // next quest
    QuestManager_gotoSubquest(interpreter->game->questManager, number);
    // return value
    STACKPUSH(interpreter, obj);
    Tn_Object_DECREF(obj);
}

void _executeFinishQuest(Interpreter* interpreter) {
    // next quest
    QuestManager_finishQuest(interpreter->game->questManager);
    // return value
    Tn_Object* result = Tn_Object_newNumber(0);
    STACKPUSH(interpreter, result);
}

void _executePrint(Interpreter* interpreter) {
    Tn_Object* text = STACKPOP(interpreter);
    printf("%s\n", (char*) text->string->data);
    // decrease refcount
    STACKPUSH(interpreter, text);
    Tn_Object_DECREF(text);
}

void _executeGoto(Interpreter* interpreter) {
    Tn_Object* ycoord = STACKPOP(interpreter);
    Tn_Object* xcoord = STACKPOP(interpreter);
    Tn_Object* character = STACKPOP(interpreter);
    // do the magic
    int px = character->character->sdlPerso->x / 32;
    int py = character->character->sdlPerso->y / 32;
    int destx = xcoord->number;
    int desty = ycoord->number;
    MapSDL* map = interpreter->game->map;

    // find the closest path
    int returnSize = 0;
    char** result = closestPath(map->map->m, map->object->m, map->object->h, map->object->l, px + 2, py + 3, destx, desty, &returnSize);
    UTF8_String* path = UTF8_empty();
    for(size_t i = 0 ; i < (size_t)returnSize ; i += 1) {
        UTF8_appendChar(path, result[i][0]);
        free(result[i]);
    }
    if(result != NULL) {
        free(result);
    }
    // animation of the npc walking
    Character_followPath(character->character, path, interpreter->game->renderer);
    // decrease refcount
    STACKPUSH(interpreter, character);
    Tn_Object_DECREF(ycoord);
    Tn_Object_DECREF(xcoord);
    Tn_Object_DECREF(character);
}

void _executePlaySound(Interpreter* interpreter) {
    Tn_Object* obj = STACKPOP(interpreter);
    UTF8_String* soundName = UTF8_copy(obj->string);
    Mix_Chunk* music = Quest_getSound(QuestManager_getCurrentQuest(interpreter->game->questManager), soundName);
    SoundManager_playMusic(interpreter->game->soundManager, music);
    UTF8_delete(soundName);
    STACKPUSH(interpreter, obj);
    Tn_Object_DECREF(obj);
}

void _executeBinaryAdd(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->add(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeBinarySub(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->sub(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeBinaryMul(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->mul(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeEqualTest(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->equal(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeNotEqualTest(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->nequal(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeGreaterOrEqualTest(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->gequal(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeLowerOrEqualTest(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->lequal(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeGreaterThanTest(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->gthan(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeLowerThanTest(Interpreter* interpreter) {
    Tn_Object* right = STACKPOP(interpreter);
    Tn_Object* left = STACKPOP(interpreter);
    Tn_Object* result = left->lthan(left, right);
    STACKPUSH(interpreter, result);
    // decrease refcount
    Tn_Object_DECREF(right);
    Tn_Object_DECREF(left);
}

void _executeJumpIfFalse(Interpreter* interpreter, int64_t number) {
    Tn_Object* value = STACKPOP(interpreter);
    if(!value->boolean) {
        interpreter->IP = number;
    }
    // decrease refcount
    Tn_Object_DECREF(value);
}

void _executeLoadVariable(Interpreter* interpreter, int64_t number) {
    Tn_Object* object = GETVARIABLE(interpreter, number);
    STACKPUSH(interpreter, object);
}

void _executeStoreVariable(Interpreter* interpreter, int64_t number) {
    Tn_Object* object = STACKPOP(interpreter);
    SETVARIABLE(interpreter, object, number);
}

void _executeCallFunction(Interpreter* interpreter, int64_t number) {
    UTF8_String* funcName = GETSTRING(interpreter, number);
    if(UTF8_equalStr(funcName, "line")) {
        _executeSayLine(interpreter);
    } else if(UTF8_equalStr(funcName, "ask")) {
        _executeAsk(interpreter);
    } else if(UTF8_equalStr(funcName, "askYesNo")) {
        _executeAskYesNo(interpreter);
    } else if(UTF8_equalStr(funcName, "askChoices")) {
        _executeAskChoices(interpreter);
    } else if(UTF8_equalStr(funcName, "give")) {
        _executeGive(interpreter);
    } else if(UTF8_equalStr(funcName, "gotoSubquest")) {
        _executeGotoSubquest(interpreter);
    } else if(UTF8_equalStr(funcName, "finishQuest")) {
        _executeFinishQuest(interpreter);
    } else if(UTF8_equalStr(funcName, "print")) {
        _executePrint(interpreter);
    } else if(UTF8_equalStr(funcName, "goto")) {
        _executeGoto(interpreter);
    } else if(UTF8_equalStr(funcName, "playsound")) {
        _executePlaySound(interpreter);
    }
}

void _executeMethodCall(Interpreter* interpreter, int64_t number) {
    ArrayList* args = ArrayList_new(Tn_Object_DECREF);
    for(size_t i = 0 ; i < (size_t) number ; i += 1) {
        ArrayList_append(args, STACKPOP(interpreter));
    }
    Tn_Object* method = STACKPOP(interpreter);
    Tn_Object* caller = STACKPOP(interpreter);
    Tn_Object* result = method->call(method, caller, args);
    STACKPUSH(interpreter, result);
    // free memory
    Tn_Object_DECREF(method);
    Tn_Object_DECREF(caller);
    ArrayList_delete(args);
}

void _executePopTop(Interpreter* interpreter) {
    Tn_Object* object = STACKPOP(interpreter);
    Tn_Object_DECREF(object);
}

void _executeDupTop(Interpreter* interpreter) {
    Tn_Object* object = STACKPOP(interpreter);
    STACKPUSH(interpreter, object);
    STACKPUSH(interpreter, object);
    // a pop = a decref
    Tn_Object_DECREF(object);
}

void _executeStoreAttribute(Interpreter* interpreter, int64_t number) {
    Tn_Object* value = STACKPOP(interpreter);
    Tn_Object* object = STACKPOP(interpreter);
    UTF8_String* name = GETSTRING(interpreter, number);
    object->setattr(object, (char*) name->data, value);
    STACKPUSH(interpreter, value);
    Tn_Object_DECREF(object);
    Tn_Object_DECREF(value);
}

void _executeLoadAttribute(Interpreter* interpreter, int64_t number) {
    Tn_Object* object = STACKPOP(interpreter);
    UTF8_String* name = GETSTRING(interpreter, number);
    Tn_Object* result = object->getattr(object, (char*) name->data);
    STACKPUSH(interpreter, result);
    Tn_Object_DECREF(object);
}

void _execute(Interpreter* interpreter, BytecodeInstruction* instruction) {
    BytecodeInstructionKind kind = instruction->kind;
    unsigned int number = instruction->number;
    switch (kind) {
        case I_LOAD_STRING:
            STACKPUSH(interpreter, Tn_Object_newString(UTF8_copy(ArrayList_get(interpreter->bytecode->strings, number))));
            break;
        case I_LOAD_NUMBER:
            STACKPUSH(interpreter, Tn_Object_newNumber(instruction->number));
            break;
        case I_BINARY_ADD:
            _executeBinaryAdd(interpreter);
            break;
        case I_BINARY_SUB:
            _executeBinarySub(interpreter);
            break;
        case I_BINARY_MUL:
            _executeBinaryMul(interpreter);
            break;
        case I_CMP_EQUAL:
            _executeEqualTest(interpreter);
            break;
        case I_CMP_NEQUAL:
            _executeNotEqualTest(interpreter);
            break;
        case I_CMP_GE:
            _executeGreaterOrEqualTest(interpreter);
            break;
        case I_CMP_LE:
            _executeLowerOrEqualTest(interpreter);
            break;
        case I_CMP_GT:
            _executeGreaterThanTest(interpreter);
            break;
        case I_CMP_LT:
            _executeLowerThanTest(interpreter);
            break;
        case I_JUMP_IF_FALSE:
            _executeJumpIfFalse(interpreter, instruction->number);
            break;
        case I_JUMP:
            interpreter->IP = number;
            break;
        case I_LOAD_VARIABLE:
            _executeLoadVariable(interpreter, instruction->number);
            break;
        case I_CALL:
            _executeCallFunction(interpreter, instruction->number);
            break;
        case I_METHOD_CALL:
            _executeMethodCall(interpreter, instruction->number);
            break;
        case I_STORE:
            _executeStoreVariable(interpreter, instruction->number);
            break;
        case I_POP_TOP:
            _executePopTop(interpreter);
            break;
        case I_DUP_TOP:
            _executeDupTop(interpreter);
            break;
        case I_LOAD_ATTR:
            _executeLoadAttribute(interpreter, instruction->number);
            break;
        case I_LOAD_METHOD:
            // _executeLoadMethod(interpreter, instruction->number);
            break;
        case I_STORE_ATTR:
            _executeStoreAttribute(interpreter, instruction->number);
            break;
        default:
            break;
    }
}

void _shutdown(Interpreter* interpreter) {
    Interpreter_reset(interpreter);
}

void Interpreter_executeNext(Interpreter* interpreter) {
    while(interpreter->state == INTERPRETER_RUNNING) {
        if(interpreter->IP >= interpreter->bytecode->instructions->length) {
            _shutdown(interpreter);
            return;
        }
        BytecodeInstruction* instruction = ArrayList_get(interpreter->bytecode->instructions, interpreter->IP);
        // printf("execute: %s\n", BytecodeInstructionKind_repr(instruction->kind));
        // Interpreter_printStack(interpreter);
        _execute(interpreter, instruction);
        interpreter->IP += 1;
    }
}

void Interpreter_resume(Interpreter* interpreter) {
    interpreter->state = INTERPRETER_RUNNING;
}

void Interpreter_tick(Interpreter* interpreter) {
    if(interpreter->state == INTERPRETER_STOPPED) {
        return;
    } else if(interpreter->state == INTERPRETER_WAITING) {
        bool canResume = interpreter->dialog == NULL || (interpreter->dialog != NULL && interpreter->dialog->state == WDG_DIALOG_ANSWER);
        if(interpreter->animation != NULL) {
            canResume &= AnimationManager_isFinished(getAnimationManager(), interpreter->animation);
        }
        if(interpreter->waitingReason == INTERPRETER_WAIT_INPUT && interpreter->dialog != NULL && interpreter->dialog->state == WDG_DIALOG_ANSWER) {
            UTF8_String* answer = UTF8_copy(interpreter->dialog->answer);
            Tn_Object* object = Tn_Object_newString(answer);
            STACKPUSH(interpreter, object);
        } else if(interpreter->animation != NULL && AnimationManager_isFinished(getAnimationManager(), interpreter->animation)) {
            interpreter->animation = NULL;
        }
        if(canResume) {
            Interpreter_resume(interpreter);
        }
    } else {
        Interpreter_executeNext(interpreter);
    }
    
}

void Interpreter_draw(Interpreter* interpreter) {
    if(interpreter->dialog == NULL) {
        return;
    } else {
        WDG_dialogDraw(interpreter->dialog, interpreter->game->renderer);
    }
}

void Interpreter_launchAnimation(Interpreter* interpreter, Animation* animation, bool wait) {
    if(wait) {
        interpreter->state = INTERPRETER_WAITING;
        interpreter->waitingReason = INTERPRETER_WAIT_ANIMATION;
        interpreter->animation = animation;
    }
    AnimationManager_launch(getAnimationManager(), animation);
}

void Interpreter_waitInput(Interpreter* interpreter) {
    interpreter->state = INTERPRETER_WAITING;
    interpreter->waitingReason = INTERPRETER_WAIT_INPUT;
}

void Interpreter_dispatchEvent(Interpreter* interpreter, SDL_Event* event) {
    if(interpreter->dialog != NULL) {
        WDG_dialogDispatchEvent(interpreter->dialog, event);
    }
}

void Interpreter_printStack(Interpreter* interpreter) {
    printf("Stack trace (%lu)\n", Stack_length(interpreter->stack));
    for(int i = Stack_length(interpreter->stack) - 1 ; i >= 0 ; i -= 1) {
        Tn_Object* string = Tn_Object_repr(ArrayList_get(interpreter->stack, i));
        printf("  %s\n", string->string->data);
        Tn_Object_delete(string);
    }
}

void Interpreter_clean(Interpreter* interpreter) {
    Stack_delete(interpreter->stack);
    HashMap_delete(interpreter->scope);
    if(interpreter->globals != NULL) {
        HashMap_delete(interpreter->globals);
    }
    if(interpreter->dialog != NULL) {
        WDG_deleteDialog(interpreter->dialog);
    }
}

void Interpreter_delete(Interpreter* interpreter) {
    Interpreter_clean(interpreter);
    Tn_Object_delete(interpreter->player);
    free(interpreter);
}