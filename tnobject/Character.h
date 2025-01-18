#ifndef TNOBJECT_CHARACTER_H
#define TNOBJECT_CHARACTER_H

#include "TnObject.h"
#include "../quest/character.h"

Tn_Object* Tn_Object_newCharacter(Character* character);
Tn_Object* Tn_Object_Character_getattr(Tn_Object* self, const char* name);
void Tn_Object_Character_setattr(Tn_Object* self, const char* name, Tn_Object* object);

#endif