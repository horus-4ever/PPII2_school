#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "quest/character.h"
#include "quest/quest.h"
#include "datastructures/stack.h"
#include "widgets/widgets.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "tnobject/objects.h"
#include "animations/animationmanager.h"

/**
 * @brief Represents the state of the interpreter.
 */
typedef enum {
    INTERPRETER_STOPPED,    ///< Interpreter is stopped.
    INTERPRETER_RUNNING,    ///< Interpreter is running.
    INTERPRETER_WAITING     ///< Interpreter is waiting.
} InterpreterState;

/**
 * @brief Represents the reason for which the interpreter is waiting.
 */
typedef enum {
    INTERPRETER_WAIT_INPUT,        ///< Interpreter is waiting for input.
    INTERPRETER_WAIT_ANIMATION     ///< Interpreter is waiting for animation.
} InterpreterWaitingReason;

/**
 * @brief Represents the interpreter for executing bytecode.
 */
typedef struct _Interpreter {
    size_t IP;                          ///< Instruction pointer.
    InterpreterState state;             ///< State of the interpreter.
    InterpreterWaitingReason waitingReason; ///< Reason for waiting.
    Character* character;               ///< Current character.
    Bytecode* bytecode;                 ///< Bytecode to execute.
    WDG_Dialog* dialog;                 ///< Dialog being displayed.
    Animation* animation;               ///< Animation being executed.
    Stack* stack;                       ///< Stack for the interpreter.
    HashMap* scope;                     ///< Scope for variables.
    HashMap* globals;                   ///< Global variables.
    struct Game* game;                  ///< Current game instance.
    Tn_Object* player;                  ///< Player object.
} Interpreter;

/**
 * @brief Pushes an object onto the interpreter's stack.
 * @param interpreter The Interpreter object.
 * @param object The object to push onto the stack.
 */
void STACKPUSH(Interpreter* interpreter, Tn_Object* object);

/**
 * @brief Pops an object from the interpreter's stack.
 * @param interpreter The Interpreter object.
 * @return The object popped from the stack.
 */
Tn_Object* STACKPOP(Interpreter* interpreter);

/**
 * @brief Creates a new Interpreter object.
 * @param player The player character.
 * @return A pointer to the newly created Interpreter object.
 */
Interpreter* Interpreter_new(Character* player);

/**
 * @brief Sets the game instance for the interpreter.
 * @param interpreter The Interpreter object.
 * @param game The game instance.
 */
void Interpreter_setGame(Interpreter* interpreter, struct Game* game);

/**
 * @brief Loads and runs bytecode in the interpreter.
 * @param interpreter The Interpreter object.
 * @param bytecode The bytecode to execute.
 */
void Interpreter_loadAndRun(Interpreter* interpreter, Bytecode* bytecode);

/**
 * @brief Sets the global variables for the interpreter.
 * @param interpreter The Interpreter object.
 * @param globals The global variables.
 */
void Interpreter_setGlobals(Interpreter* interpreter, HashMap* globals);

/**
 * @brief Updates the interpreter.
 * @param interpreter The Interpreter object.
 */
void Interpreter_tick(Interpreter* interpreter);

/**
 * @brief Executes the next instruction in the interpreter.
 * @param interpreter The Interpreter object.
 */
void Interpreter_executeNext(Interpreter* interpreter);

/**
 * @brief Resumes execution of the interpreter.
 * @param interpreter The Interpreter object.
 */
void Interpreter_resume(Interpreter* interpreter);

/**
 * @brief Draws the interpreter's output.
 * @param interpreter The Interpreter object.
 */
void Interpreter_draw(Interpreter* interpreter);

/**
 * @brief Dispatches an event to the interpreter.
 * @param interpreter The Interpreter object.
 * @param event The SDL event.
 */
void Interpreter_dispatchEvent(Interpreter* interpreter, SDL_Event* event);

/**
 * @brief Deletes the Interpreter object and frees its resources.
 * @param interpreter The Interpreter object to delete.
 */
void Interpreter_delete(Interpreter* interpreter);

/**
 * @brief Cleans up the Interpreter object.
 * @param interpreter The Interpreter object to clean.
 */
void Interpreter_clean(Interpreter* interpreter);

/**
 * @brief Resets the state of the Interpreter object.
 * @param interpreter The Interpreter object to reset.
 */
void Interpreter_reset(Interpreter* interpreter);

/**
 * @brief Launches an animation in the interpreter.
 * @param interpreter The Interpreter object.
 * @param animation The animation to launch.
 * @param wait Whether to wait for the animation to finish.
 */
void Interpreter_launchAnimation(Interpreter* interpreter, Animation* animation, bool wait);

/**
 * @brief Waits for input in the interpreter.
 * @param interpreter The Interpreter object.
 */
void Interpreter_waitInput(Interpreter* interpreter);

/**
 * @brief Prints the contents of the interpreter's stack.
 * @param interpreter The Interpreter object.
 */
void Interpreter_printStack(Interpreter* interpreter);

#endif