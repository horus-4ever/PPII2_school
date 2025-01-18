#include "Number.h"
#include "objects.h"

Tn_Object* Tn_Object_newNumber(int64_t value) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = NUMBER_OBJ;
    object->number = value;
    object->getattr = Tn_Object_Number_getattr;
    object->equal = Tn_Object_Number_equal;
    object->add = Tn_Object_Number_add;
    object->sub = Tn_Object_Number_sub;
    object->mul = Tn_Object_Number_mul;
    object->gequal = Tn_Object_Number_gequal;
    object->lequal = Tn_Object_Number_lequal;
    object->gthan = Tn_Object_Number_gthan;
    object->lthan = Tn_Object_Number_lthan;
    return object;
}

Tn_Object* Tn_Object_Number_equal(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(self->number == object->number);
}

Tn_Object* Tn_Object_Number_gequal(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(self->number >= object->number);
}

Tn_Object* Tn_Object_Number_lequal(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(self->number <= object->number);
}

Tn_Object* Tn_Object_Number_gthan(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(self->number > object->number);
}

Tn_Object* Tn_Object_Number_lthan(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(self->number < object->number);
}

Tn_Object* Tn_Object_Number_add(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newNumber(self->number + object->number);
}

Tn_Object* Tn_Object_Number_sub(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newNumber(self->number - object->number);
}

Tn_Object* Tn_Object_Number_mul(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newNumber(self->number * object->number);
}

Tn_Object* Tn_Object_Number_toString(Tn_Object* self, ArrayList* args) {
    char buffer[128] = {0};
    sprintf(buffer, "%ld", self->number);
    return Tn_Object_newString(UTF8_from(buffer));
}

Tn_Object* Tn_Object_Number_getattr(Tn_Object* self, const char* name) {
    if(strcmp(name, "toString") == 0) {
        return Tn_Object_newFunction(Tn_Object_Number_toString);
    }
    Tn_Object_ATTRIBUTE_ERROR(self, name);
    return NULL;
}