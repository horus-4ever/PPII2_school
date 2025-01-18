#ifndef TNOBJECT_FUNCTION_H
#define TNOBJECT_FUNCTION_H

#include "TnObject.h"

Tn_Object* Tn_Object_newFunction(Tn_Object*(*func)());

#endif