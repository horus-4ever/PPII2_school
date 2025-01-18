#include "Player.h"
#include "objects.h"

Tn_Object* Tn_Object_newPlayer(Perso_SDL* player) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = PLAYER_OBJ;
    object->player = player;
    object->getattr = Tn_Object_Player_getattr;
    object->setattr = Tn_Object_Player_setattr;
    return object;
}

Tn_Object* Tn_Object_Player_toString(Tn_Object* self, ArrayList* args) {
    return self->repr(self);
}

Tn_Object* Tn_Object_Player_getattr(Tn_Object* self, const char* name) {
    if(strcmp(name, "x") == 0) {
        return Tn_Object_newNumber(self->player->x / BLOC_SIZE);
    } else if(strcmp(name, "y") == 0) {
        return Tn_Object_newNumber(self->player->y / BLOC_SIZE);
    } else if (strcmp(name, "toString") == 0) {
        return Tn_Object_newFunction(Tn_Object_Player_toString);
    } else {
        Tn_Object_ATTRIBUTE_ERROR(self, name);
    }
    return NULL;
}

void Tn_Object_Player_setattr(Tn_Object* self, const char* name, Tn_Object* object) {
    if(strcmp(name, "x") == 0) {
        self->player->x = object->number;
    } else {
        Tn_Object_ATTRIBUTE_ERROR(self, name);
    }
    Tn_Object_DECREF(object);
}