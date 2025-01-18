#include "TnObject.h"
#include "../datastructures/array.h"
#include "objects.h"
#include <stdlib.h>
#include <string.h>

void Tn_Object_ATTRIBUTE_ERROR(Tn_Object* object, const char* name) {
    printf("Interpreter error\n");
    printf("  -> '%s' object has no attribute '%s'\n", Tn_ObjectType_repr(object->type), name);
    exit(-1);
}


void Tn_Object_INCREF(Tn_Object* object) {
    object->refcount += 1;
}

void Tn_Object_DECREF(Tn_Object* object) {
    object->refcount -= 1;
    if(object->refcount <= 0) {
        Tn_Object_delete(object);
    }
}

void Tn_Object_delete(Tn_Object* object) {
    switch (object->type) {
    case STRING_OBJ:
        UTF8_delete(object->string);
        break;
    case BOOL_OBJ:
        break;
    case PLAYER_OBJ:
        free_perso(object->player);
        break;
    case CHARACTER_OBJ:
        break;
    default:
        break;
    }
    free(object);
}

const char* Tn_ObjectType_repr(Tn_ObjectType type) {
    switch (type) {
    case PLAYER_OBJ:
        return "Player";
    case STRING_OBJ:
        return "String";
    case NUMBER_OBJ:
        return "Number";
    case BOOL_OBJ:
        return "Boolean";
    case FUNCTION_OBJ:
        return "Function";
    case CHARACTER_OBJ:
        return "Character";
    case QUEST_OBJ:
        return "Quest";
    default:
        break;
    }
    return NULL; // unreachable
}

void Tn_Object_init(Tn_Object* object) {
    object->refcount = 0;
    object->add = Tn_Object_add;
    object->equal = Tn_Object_equal;
    object->nequal = Tn_Object_nequal;
    object->gequal = Tn_Object_gequal;
    object->lequal = Tn_Object_lequal;
    object->gthan = Tn_Object_gthan;
    object->lthan = Tn_Object_lthan;
    object->getattr = Tn_Object_getattr;
    object->setattr = Tn_Object_setattr;
    object->sub = Tn_Object_sub;
    object->mul = Tn_Object_mul;
    object->repr = Tn_Object_repr;
}

Tn_Object* Tn_Object_repr(Tn_Object* self) {
    UTF8_String* result = UTF8_from("<object '");
    UTF8_append(result, Tn_ObjectType_repr(self->type));
    UTF8_append(result, "' at ");
    char buffer[1024] = {0};
    sprintf(buffer, "%p", (void*) self);
    UTF8_append(result, buffer);
    UTF8_append(result, ">");
    return Tn_Object_newString(result);
}

Tn_Object* Tn_Object_getattr(Tn_Object* self, const char* name) {
    Tn_Object_ATTRIBUTE_ERROR(self, name);
    return NULL;
}

void Tn_Object_setattr(Tn_Object* self, const char* name, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, name);
}

Tn_Object* Tn_Object_equal(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, "==");
    return NULL;
}

Tn_Object* Tn_Object_add(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, "+");
    return NULL;
}

Tn_Object* Tn_Object_sub(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, "-");
    return NULL;
}

Tn_Object* Tn_Object_mul(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, "*");
    return NULL;
}

Tn_Object* Tn_Object_gequal(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, ">=");
    return NULL;
}

Tn_Object* Tn_Object_lequal(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, "<=");
    return NULL;
}

Tn_Object* Tn_Object_gthan(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, ">");
    return NULL;
}

Tn_Object* Tn_Object_lthan(Tn_Object* self, Tn_Object* object) {
    Tn_Object_ATTRIBUTE_ERROR(self, "<");
    return NULL;
}

Tn_Object* Tn_Object_nequal(Tn_Object* self, Tn_Object* object) {
    Tn_Object* result = self->equal(self, object);
    result->boolean = !result->boolean;
    return result;
}