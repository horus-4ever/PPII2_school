#ifndef QUESTMANAGER_H
#define QUESTMANAGER_H

#include "quest.h"
#include "../utf8strings/utf8strings.h"

/**
 * Structure representing the QuestManager.
 */
typedef struct QuestManager {
    Quest* current; /**< Pointer to the current Quest. */
    struct Game* game; /**< Pointer to the Game structure. */
} QuestManager;

/**
 * Creates a new QuestManager object.
 * @return A pointer to the newly created QuestManager object.
 */
QuestManager* QuestManager_new(void);

/**
 * Sets the game associated with the QuestManager.
 * @param manager A pointer to the QuestManager object.
 * @param game A pointer to the Game structure.
 */
void QuestManager_setGame(QuestManager* manager, struct Game* game);

/**
 * Loads a quest from the specified path.
 * @param manager A pointer to the QuestManager object.
 * @param path A pointer to the UTF8_String containing the path to the quest.
 */
void QuestManager_load(QuestManager* manager, UTF8_String* path);

/**
 * Loads the next quest.
 * @param manager A pointer to the QuestManager object.
 */
void QuestManager_loadNext(QuestManager* manager);

/**
 * Animates the current quest.
 * @param manager A pointer to the QuestManager object.
 */
void QuestManager_animate(QuestManager* manager);

/**
 * Moves to the specified subquest within the current quest.
 * @param manager A pointer to the QuestManager object.
 * @param number The number of the subquest to move to.
 */
void QuestManager_gotoSubquest(QuestManager* manager, size_t number);

/**
 * Finishes the current quest.
 * @param manager A pointer to the QuestManager object.
 */
void QuestManager_finishQuest(QuestManager* manager);

/**
 * Gets the current quest.
 * @param manager A pointer to the QuestManager object.
 * @return A pointer to the current Quest.
 */
Quest* QuestManager_getCurrentQuest(QuestManager* manager);

/**
 * Adjusts the NPC position relative to the main player.
 * @param manager A pointer to the QuestManager object.
 * @param mc A pointer to the Perso_SDL representing the main player.
 */
void QuestManager_adjustNpcPositionRelativeToMainPlayer(QuestManager* manager, Perso_SDL* mc);

/**
 * Updates the QuestManager.
 * @param manager A pointer to the QuestManager object.
 */
void QuestManager_update(QuestManager* manager);

/**
 * Draws the QuestManager.
 * @param manager A pointer to the QuestManager object.
 */
void QuestManager_draw(QuestManager* manager);

/**
 * Deletes the QuestManager object and frees associated resources.
 * @param manager A pointer to the QuestManager object to be deleted.
 */
void QuestManager_delete(QuestManager* manager);

/**
 * Requests interaction with an NPC.
 * @param manager A pointer to the QuestManager object.
 * @param perso A pointer to the Perso_SDL representing the NPC.
 */
void QuestManager_requestInteraction(QuestManager* manager, Perso_SDL* perso);

ArrayList* QuestManager_getCharacters(QuestManager* manager);

#endif