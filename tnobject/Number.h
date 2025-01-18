#ifndef TNOBJECT_NUMBER_H
#define TNOBJECT_NUMBER_H

#include "TnObject.h"

Tn_Object* Tn_Object_newNumber(int64_t value);
Tn_Object* Tn_Object_Number_getattr(Tn_Object* self, const char* name);
Tn_Object* Tn_Object_Number_equal(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_gequal(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_lequal(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_gthan(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_lthan(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_add(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_sub(Tn_Object* self, Tn_Object* object);
Tn_Object* Tn_Object_Number_mul(Tn_Object* self, Tn_Object* object);

#endif