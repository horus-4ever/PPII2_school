#include "character.h"
#include "../tnobject/objects.h"
#include "../animations/animationmanager.h"
#include "../animations/npcanimation.h"

Character* Character_new(UTF8_String* name, UTF8_String* skin, int spawnx, int spawny, int orientation, Bytecode* bytecode, SDL_Renderer* renderer) {
    Perso_SDL* npc = init_perso(spawnx+512, spawny+544, SIZE_PERSO, SIZE_PERSO, orientation, (char*) skin->data, renderer, 8, 5);
    Character* result = malloc(sizeof(Character));
    result->name = name;
    result->bytecode = bytecode;
    result->sdlPerso = npc;
    result->skinName = skin;
    result->talked = false;
    result->animation = NULL;
    Tn_Object* object = Tn_Object_newCharacter(result);
    Tn_Object_INCREF(object);
    Bytecode_setContextObject(bytecode, object);
    return result;
}

Character* Character_fromSdlPerso(Perso_SDL* perso) {
    Character* result = malloc(sizeof(Character));
    result->name = UTF8_from("Crée ton avatar personnalisé !");
    result->bytecode = NULL;
    result->sdlPerso = perso;
    result->skinName = UTF8_empty();
    result->talked = false;
    result->animation = NULL;
    return result;
}

void Character_setName(Character* character, UTF8_String* name) {
    UTF8_delete(character->name);
    character->name = name;
}

void Character_draw(Character* character, SDL_Renderer* renderer) {
    Perso_SDL* perso = character->sdlPerso;
    SDL_RenderCopy(renderer, perso->sprite, &perso->srcRect, &perso->dstRect);
}

void Character_updatePosition(Character* character, int x, int y) {
    character->sdlPerso->dstRect.x += x;
    character->sdlPerso->dstRect.y += y;
}

bool Character_talkedWith(Character* character) {
    return character->talked;
}

void Character_followPath(Character* character, UTF8_String* path, SDL_Renderer* renderer) {
    NPCAnimationContext* context = NPCAnimationContext_new(character, path);
    Animation* animation = NPCAnimation_new(context);
    AnimationManager* manager = getAnimationManager();
    AnimationManager_launch(manager, animation);
}

void Character_adjustRelativeToMainPlayer(Character* character, Perso_SDL* mc) {
    Perso_SDL* perso = character->sdlPerso;
    int dx = mc->x - perso->x;
    int dy = mc->y - perso->y;
    perso->dstRect.x = mc->dstRect.x - dx;
    perso->dstRect.y = mc->dstRect.y - dy;
}

void Character_delete(Character* character) {
    UTF8_delete(character->name);
    UTF8_delete(character->skinName);
    if(character->bytecode != NULL) {
        Bytecode_delete(character->bytecode);
    }
    free_perso(character->sdlPerso);
    if(character->animation != NULL) {
        AnimationManager_stop(getAnimationManager(), character->animation);
    }
    free(character);
}