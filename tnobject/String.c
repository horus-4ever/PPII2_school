#include "String.h"
#include "objects.h"

Tn_Object* Tn_Object_newString(UTF8_String* value) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = STRING_OBJ;
    object->string = value;
    object->getattr = Tn_Object_String_getattr;
    object->equal = Tn_Object_String_equal;
    object->add = Tn_Object_String_add;
    return object;
}

Tn_Object* Tn_Object_String_equal(Tn_Object* self, Tn_Object* object) {
    return Tn_Object_newBoolean(UTF8_equal(self->string, object->string));
}

Tn_Object* Tn_Object_String_add(Tn_Object* self, Tn_Object* object) {
    UTF8_String* result = UTF8_copy(self->string);
    UTF8_append(result, (char*) object->string->data);
    return Tn_Object_newString(result);
}

Tn_Object* Tn_Object_String_getattr(Tn_Object* self, const char* name) {
    if(strcmp(name, "length") == 0) {
        return Tn_Object_newNumber(self->string->length);
    }
    Tn_Object_ATTRIBUTE_ERROR(self, name);
    return NULL;
}