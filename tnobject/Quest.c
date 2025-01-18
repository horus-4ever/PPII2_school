#include "Quest.h"
#include "objects.h"

Tn_Object* Tn_Object_newQuest(Quest* quest) {
    Tn_Object* object = malloc(sizeof(Tn_Object));
    Tn_Object_init(object);
    object->type = QUEST_OBJ;
    object->quest = quest;
    object->getattr = Tn_Object_Quest_getattr;
    object->setattr = Tn_Object_Quest_setattr;
    return object;
}

Tn_Object* Tn_Object_Quest_toString(Tn_Object* self, ArrayList* args) {
    return self->repr(self);
}

Tn_Object* Tn_Object_Quest_getattr(Tn_Object* self, const char* name) {
    if(strcmp(name, "subquest") == 0) {
        return Tn_Object_newNumber(self->quest->currentSubquest);
    }
    return NULL;
}

void Tn_Object_Quest_setattr(Tn_Object* self, const char* name, Tn_Object* object) {
    Tn_Object_DECREF(object);
}