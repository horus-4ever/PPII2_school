#ifndef TNOBJECT_PLAYER_H
#define TNOBJECT_PLAYER_H

#include "TnObject.h"

Tn_Object* Tn_Object_newPlayer(Perso_SDL* player);
Tn_Object* Tn_Object_Player_getattr(Tn_Object* self, const char* name);
void Tn_Object_Player_setattr(Tn_Object* self, const char* name, Tn_Object* object);

#endif