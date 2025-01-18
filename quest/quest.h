#ifndef QUEST_H
#define QUEST_H

#include "../utf8strings/utf8strings.h"
#include "../datastructures/array.h"
#include "../widgets/widgets.h"
#include "../perso_SDL.h"
#include "../items.h"
#include "character.h"
#include "../questparser/compiler.h"
#include <SDL2/SDL_mixer.h>

struct _Interpreter;
struct Tn_Object;

typedef enum {
    QUESTION_YES_NO,
    QUESTION_MULTIPLE_CHOICE
} QuestionType;

typedef struct {
    QuestionType type;
    union {
        struct {
            bool yes;
            bool no;
        } yesNoQuestion;
        struct {
            char** choices;
            int numChoices;
            int correctChoice;
        } multipleChoiceQuestion;
    } data;
} Question;

typedef struct {
    char* sender;
    char* recipient;
    char* content;
    Question* question; // Pointeur vers la question associée, si applicable
} Message;

typedef enum {
    GOAL_FIND_OBJECT,
    GOAL_FIND_CHARACTER,
    GOAL_ANSWER_QUESTION,
    GOAL_SOLVE_PUZZLE,
    GOAL_TALK_TO_CHARACTER,
    GOAL_EXPLORE_LOCATION
} QuestGoalType;

typedef struct {
    int number;
    QuestGoalType goal;
    UTF8_String* description;
    Bytecode* initCode;
    Bytecode* loopCode;
} Subquest;

typedef struct {
    UTF8_String* name;
    UTF8_String* map;
    UTF8_String* nextquest;
    size_t number;
    int difficulty;
} QuestInfo;

typedef struct Quest {
    QuestInfo infos;
    ArrayList* characters;
    ArrayList* subquests;
    WDG_TextArea* textarea;
    WDG_Button* interact;
    HashMap* sounds;
    Items* items;
    bool completed;
    size_t currentSubquest;
} Quest;

void QuestInfo_delete(QuestInfo* infos);

Quest* readQuestFile(const char* filename, SDL_Renderer* renderer);
Quest* Quest_new(QuestInfo infos, ArrayList* characters, ArrayList* subquests, Items* items, HashMap* sounds);
Mix_Chunk* Quest_getSound(Quest* quest, UTF8_String* name);
bool Quest_isCompleted(Quest* quest);
void Quest_update(Quest* quest);
void Quest_updateNpcPosition(Quest* quest, int x, int y);
void Quest_adjustNpcPositionRelativeToMainPlayer(Quest* quest, Perso_SDL* mc);
void Quest_gotoSubquest(Quest* quest, size_t number);
void Quest_finishQuest(Quest* quest);
void Quest_findItem(Quest* quest, UTF8_String* itemName);
Subquest* Quest_getCurrentSubquest(Quest* quest);
HashMap* Quest_createGlobals(Quest* quest);
void Quest_draw(Quest* quest, SDL_Renderer* renderer);
Character* Quest_findAnyInteraction(Quest* quest, Perso_SDL* player);
void Quest_delete(Quest* quest);


Subquest* Subquest_new(int number, QuestGoalType goal, UTF8_String* description, Bytecode* initCode, Bytecode* loopCode);
void Subquest_delete(Subquest* subquest);

#endif