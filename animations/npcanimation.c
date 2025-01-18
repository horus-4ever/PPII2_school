#include "npcanimation.h"
#include "../npc.h"

NPCAnimationContext* NPCAnimationContext_new(Character* character, UTF8_String* path) {
    NPCAnimationContext* result = malloc(sizeof(NPCAnimationContext));
    result->path = path;
    result->character = character;
    return result;
}

void NPCAnimationContext_delete(NPCAnimationContext* context) {
    UTF8_delete(context->path);
    free(context);
}

Animation* NPCAnimation_new(NPCAnimationContext* context) {
    size_t length = UTF8_length(context->path);
    return Animation_new(length * 8, false, NPCAnimation_tick, NPCAnimation_draw, NPCAnimation_delete, context);
}

void NPCAnimation_tick(Animation* animation) {
    if(animation->count % 8 != 0) {
        return;
    }
    NPCAnimationContext* context = animation->context;
    size_t frame = animation->count / 8;
    Perso_SDL* perso = context->character->sdlPerso;
    switch (context->path->data[frame]) {
    case 'U':
        update_position_specific_npc(perso, perso->x, perso->y - PERSO_SPEED);
        break;
    case 'D':
        update_position_specific_npc(perso, perso->x, perso->y + PERSO_SPEED);
        break;
    case 'L':
        update_position_specific_npc(perso, perso->x - PERSO_SPEED, perso->y);
        break;
    case 'R':
        update_position_specific_npc(perso, perso->x + PERSO_SPEED, perso->y);
        break;
    default:
        break;
    }
}

void NPCAnimation_draw(Animation* animation, SDL_Renderer* renderer) {

}

void NPCAnimation_delete(Animation* animation) {
    NPCAnimationContext_delete(animation->context);
}