#ifndef TNOBJECT_BOOLEAN_H
#define TNOBJECT_BOOLEAN_H

#include "TnObject.h"

Tn_Object* Tn_Object_newBoolean(bool value);
Tn_Object* Tn_Object_Boolean_equal(Tn_Object* self, Tn_Object* object);

#endif