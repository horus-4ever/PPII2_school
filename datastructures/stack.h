#ifndef STACK_H
#define STACK_H

#include "array.h"
#include "common.h"

/**
 * Stack type alias for ArrayList.
 */
typedef ArrayList Stack;

/**
 * Creates a new stack.
 * @param deleter A function pointer to the deleter function for freeing elements.
 * @return A pointer to the newly created stack.
 */
Stack* Stack_new(deleter_f deleter);

/**
 * Pushes a value onto the stack.
 * @param stack A pointer to the stack.
 * @param value A pointer to the value to be pushed onto the stack.
 */
void Stack_push(Stack* stack, void* value);

/**
 * Pops and returns the top value from the stack.
 * @param stack A pointer to the stack.
 * @return A pointer to the popped value.
 */
void* Stack_pop(Stack* stack);

/**
 * Clears the stack, removing all elements.
 * @param stack A pointer to the stack.
 */
void Stack_clear(Stack* stack);

/**
 * Deletes the stack and frees associated resources.
 * @param stack A pointer to the stack to be deleted.
 */
void Stack_delete(Stack* stack);

/**
 * Returns the length of the stack.
 * @param stack A pointer to the stack.
 * @return The length of the stack.
 */
size_t Stack_length(Stack* stack);

#endif