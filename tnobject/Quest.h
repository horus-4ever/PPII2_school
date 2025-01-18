#ifndef TNOBJECT_QUEST_H
#define TNOBJECT_QUEST_H

#include "TnObject.h"

Tn_Object* Tn_Object_newQuest(Quest* quest);
Tn_Object* Tn_Object_Quest_getattr(Tn_Object* self, const char* name);
void Tn_Object_Quest_setattr(Tn_Object* self, const char* name, Tn_Object* object);

#endif