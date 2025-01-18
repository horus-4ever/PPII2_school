#include "Function.h"

Tn_Object* Tn_Object_newFunction(Tn_Object*(*func)()) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = FUNCTION_OBJ;
    object->function = func;
    object->refcount = 0;
    object->call = Tn_Object_Function_call;
    return object;
}

Tn_Object* Tn_Object_Function_call(Tn_Object* self, Tn_Object* object, ArrayList* args) {
    return self->function(object, args);
}