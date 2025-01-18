#include "questmanager.h"
#include "../game.h"
#include "../animations/questanimation.h"
#include "../animations/endanimation.h"
#include <stdlib.h>

QuestManager* QuestManager_new(void) {
    QuestManager* result = malloc(sizeof(QuestManager));
    result->current = NULL;
    result->game = NULL;
    return result;
}

void QuestManager_setGame(QuestManager* manager, Game* game) {
    manager->game = game;
}

void QuestManager_load(QuestManager* manager, UTF8_String* path) {
    if(manager->current != NULL) {
        Quest_delete(manager->current);
    }
    Quest* quest = readQuestFile(UTF8_str(path), manager->game->renderer);
    manager->current = quest;
    HashMap* globals = Quest_createGlobals(quest);
    Interpreter_setGlobals(manager->game->interpreter, globals);
    Interpreter_loadAndRun(manager->game->interpreter, Quest_getCurrentSubquest(quest)->initCode);
    QuestManager_animate(manager);
}

void QuestManager_loadNext(QuestManager* manager) {
    if(manager->current != NULL && manager->current->infos.nextquest != NULL) {
        UTF8_String* nextQuest = UTF8_from("ressources/quests/");
        UTF8_append(nextQuest, (char*) manager->current->infos.nextquest->data);
        QuestManager_load(manager, nextQuest);
        UTF8_delete(nextQuest);
    } else if(manager->current->infos.nextquest == NULL) {
        EndAnimationContext* context = EndAnimationContext_new(UTF8_from("The END"));
        Animation* animation = EndAnimation_new(context);
        AnimationManager* manager = getAnimationManager();
        AnimationManager_launch(manager, animation);
        Game_setState(manager->game, GAME_FINISHED);
    }
}

void QuestManager_animate(QuestManager* manager) {
    Quest* quest = manager->current;
    QuestAnimationContext* context = QuestAnimationContext_new(quest->infos.name);
    Animation* animation = QuestAnimation_new(context);
    AnimationManager_launch(getAnimationManager(), animation);
}

Quest* QuestManager_getCurrentQuest(QuestManager* manager) {
    return manager->current;
}

void QuestManager_gotoSubquest(QuestManager* manager, size_t number) {
    Quest_gotoSubquest(manager->current, number);
    if(Quest_isCompleted(manager->current)) {
        QuestManager_loadNext(manager);
    } else {
        Subquest* subquest = Quest_getCurrentSubquest(manager->current);
        Interpreter_loadAndRun(manager->game->interpreter, subquest->initCode);
    }
}

void QuestManager_finishQuest(QuestManager* manager) {
    Quest_finishQuest(manager->current);
    QuestManager_loadNext(manager);
}

void QuestManager_adjustNpcPositionRelativeToMainPlayer(QuestManager* manager, Perso_SDL* mc) {
    Quest_adjustNpcPositionRelativeToMainPlayer(manager->current, mc);
}

void QuestManager_requestInteraction(QuestManager* manager, Perso_SDL* perso) {
    Character* character = Quest_findAnyInteraction(manager->current, perso);
    if(character == NULL) {
        return;
    }
    Interpreter_loadAndRun(manager->game->interpreter, character->bytecode);
}

void QuestManager_update(QuestManager* manager) {
    Quest_update(manager->current);
    if(!Quest_isCompleted(manager->current)) {
        Interpreter_loadAndRun(manager->game->interpreter, Quest_getCurrentSubquest(manager->current)->loopCode);
    }
}

void QuestManager_draw(QuestManager* manager) {
    Quest_draw(manager->current, manager->game->renderer);
}

void QuestManager_delete(QuestManager* manager) {
    if(manager->current != NULL) {
        Quest_delete(manager->current);
    }
    free(manager);
}

ArrayList* QuestManager_getCharacters(QuestManager* manager) {
    return manager->current->characters;
}