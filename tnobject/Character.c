#include "Character.h"
#include "Function.h"
#include "Boolean.h"
#include "Number.h"
#include "String.h"

Tn_Object* Tn_Object_newCharacter(Character* character) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = CHARACTER_OBJ;
    object->character = character;
    object->getattr = Tn_Object_Character_getattr;
    object->setattr = Tn_Object_Character_setattr;
    return object;
}

Tn_Object* Tn_Object_Character_toString(Tn_Object* self, ArrayList* args) {
    return self->repr(self);
}

Tn_Object* Tn_Object_Character_goto(Tn_Object* self, ArrayList* args) {
    Tn_Object* xcoord = ArrayList_get(args, 0);
    return xcoord;
}

Tn_Object* Tn_Object_Character_interacted(Tn_Object* self, ArrayList* args) {
    bool interacted = Character_talkedWith(self->character);
    return Tn_Object_newBoolean(interacted);
}

Tn_Object* Tn_Object_Character_getattr(Tn_Object* self, const char* name) {
    if(strcmp(name, "x") == 0) {
        return Tn_Object_newNumber(self->character->sdlPerso->x / 32);
    } else if(strcmp(name, "y") == 0) {
        return Tn_Object_newNumber(self->character->sdlPerso->y / 32);
    } else if(strcmp(name, "toString") == 0) {
        return Tn_Object_newFunction(Tn_Object_Character_toString);
    } else if(strcmp(name, "goto") == 0) {
        return Tn_Object_newFunction(Tn_Object_Character_goto);
    } else if(strcmp(name, "interacted") == 0) {
        return Tn_Object_newFunction(Tn_Object_Character_interacted);
    } else if(strcmp(name, "name") == 0) {
        return Tn_Object_newString(UTF8_copy(self->character->name));
    }
    else {
        Tn_Object_ATTRIBUTE_ERROR(self, name);
    }
    return NULL;
}

void Tn_Object_Character_setattr(Tn_Object* self, const char* name, Tn_Object* object) {
    if(strcmp(name, "name") == 0) {
        Character_setName(self->character, UTF8_copy(object->string));
    } else {
        Tn_Object_ATTRIBUTE_ERROR(self, name);
    }
}