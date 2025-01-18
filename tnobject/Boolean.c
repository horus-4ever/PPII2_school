#include "Boolean.h"

Tn_Object* Tn_Object_newBoolean(bool value) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = BOOL_OBJ;
    object->boolean = value;
    object->equal = Tn_Object_Boolean_equal;
    return object;
}

Tn_Object* Tn_Object_Boolean_equal(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(self->boolean == object->boolean);
}