#include "stack.h"

Stack* Stack_new(deleter_f deleter) {
    return ArrayList_new(deleter);
}

void Stack_push(Stack* stack, void* value) {
    ArrayList_append(stack, value);
}

void* Stack_pop(Stack* stack) {
    return ArrayList_pop(stack);
}

size_t Stack_length(Stack* stack) {
    return ArrayList_length(stack);
}

void Stack_clear(Stack* stack) {
    ArrayList_clear(stack);
}

void Stack_delete(Stack* stack) {
    ArrayList_delete(stack);
}