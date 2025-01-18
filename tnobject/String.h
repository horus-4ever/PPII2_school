#ifndef TNOBJECT_STRING_H
#define TNOBJECT_STRING_H

#include "TnObject.h"

Tn_Object* Tn_Object_newString(UTF8_String* value);
Tn_Object* Tn_Object_String_getattr(Tn_Object* self, const char* name);
Tn_Object* Tn_Object_String_equal(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_String_add(Tn_Object* self, Tn_Object* object);

#endif