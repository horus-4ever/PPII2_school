#include "quest.h"
#include "../questparser/parser.h"
#include "../questparser/tokens.h"
#include "../interpreter.h"
#include "../npc.h"
#include "../tnobject/TnObject.h"
#include "../animations/animationmanager.h"
#include "../animations/questanimation.h"

Quest* readQuestFile(const char* filename, SDL_Renderer* renderer) {
    UTF8_String* fileContent = UTF8_fromFile(filename);
    Quest* result = parseQuest((char*) fileContent->data, renderer);
    UTF8_delete(fileContent);
    return result;
}

Quest* Quest_new(QuestInfo infos, ArrayList* characters, ArrayList* subquests, Items* items, HashMap* sounds) {
    Quest* result = malloc(sizeof(Quest));
    result->infos = infos;
    result->characters = characters;
    result->subquests = subquests;
    result->items = items;
    result->completed = false;
    result->currentSubquest = 1;
    result->sounds = sounds;
    // textarea
    WDG_TextArea* textarea = WDG_createTextArea(UTF8_from("default"), WDG_getFont(DEFAULT_FONT, 28), WIDTH / 2, 50);
    WDG_textareaSetBackgroundColor(textarea, WDG_TRANSPARENT);
    WDG_textareaSetFontColor(textarea, WDG_BLACK);
    WDG_textareaSetPos(textarea, 25, 120);
    result->textarea = textarea;
    // button
    WDG_Button* button = WDG_createTextButton(UTF8_from("interact"), WDG_getFont(DEFAULT_FONT, 35), 200, 30);
    WDG_buttonSetBackgroundColor(button, WDG_TRANSPARENT);
    WDG_buttonSetFontColor(button, WDG_GREY);
    WDG_buttonSetPos(button, 20, HEIGHT - 40);
    WDG_buttonSetBorder(button, WDG_NONE);
    WDG_buttonSetState(button, WDG_HIDDEN);
    result->interact = button;
    return result;
}

Subquest* Subquest_new(int number, QuestGoalType goal, UTF8_String* description, Bytecode* initCode, Bytecode* loopCode) {
    Subquest* result = malloc(sizeof(Subquest));
    result->number = number;
    result->goal = goal;
    result->description = description;
    result->initCode = initCode;
    result->loopCode = loopCode;
    return result;
}

Mix_Chunk* Quest_getSound(Quest* quest, UTF8_String* name) {
    return HashMap_get(quest->sounds, name);
}

HashMap* Quest_createGlobals(Quest* quest) {
    HashMap* globals = HashMap_new(UTF8_delete, Tn_Object_DECREF, UTF8_equal, UTF8_hash);
    for(size_t i = 0 ; i < ArrayList_length(quest->characters) ; i += 1) {
        Character* character = ArrayList_get(quest->characters, i);
        Tn_Object* characterObj = Tn_Object_newCharacter(character);
        Tn_Object_INCREF(characterObj);
        HashMap_set(globals, UTF8_copy(character->name), characterObj);
    }
    Tn_Object* questObj = Tn_Object_newQuest(quest);
    Tn_Object_INCREF(questObj);
    HashMap_set(globals, UTF8_from("quest"), questObj);
    return globals;
}

void Quest_update(Quest* quest) {
    /* if(quest->currentSubquest >= ArrayList_length(quest->subquests)) {
        quest->completed = true;
    } */
}

bool Quest_isCompleted(Quest* quest) {
    return quest->completed;
}

void Quest_findItem(Quest* quest, UTF8_String* itemName) {
    for(int i = 0 ; i < quest->items->nb_items ; i += 1) {
        if(UTF8_equal(quest->items->names[i], itemName)) {
            get_item(quest->items, i);
            break;
        }
    }
}

Subquest* Quest_getCurrentSubquest(Quest* quest) {
    for(size_t i = 0 ; i < ArrayList_length(quest->subquests) ; i += 1) {
        Subquest* subquest = ArrayList_get(quest->subquests, i);
        if((size_t) subquest->number == quest->currentSubquest) {
            return ArrayList_get(quest->subquests, i);
        }
    }
    return NULL;
}

void Quest_finishQuest(Quest* quest) {
    quest->completed = true;
}

void Quest_gotoSubquest(Quest* quest, size_t number) {
    for(size_t i = 0 ; i < ArrayList_length(quest->characters) ; i += 1) {
        Character* character = ArrayList_get(quest->characters, i);
        character->talked = false;
    }
    for(size_t i = 0 ; i < ArrayList_length(quest->subquests) ; i += 1) {
        Subquest* subquest = ArrayList_get(quest->subquests, i);
        if(subquest->number == (int) number) {
            quest->currentSubquest = number;
        }
    }
}

void Quest_draw(Quest* quest, SDL_Renderer* renderer) {
    if(quest->completed) {
        return;
    }
    Subquest* subquest = Quest_getCurrentSubquest(quest);
    WDG_textareaSetText(quest->textarea, UTF8_copy(subquest->description));
    WDG_drawTextArea(quest->textarea, renderer);
    WDG_drawButton(quest->interact, renderer);
    // NPC_SDL_RenderCopy(quest->characters, renderer);
    render_items(quest->items, renderer);
}

void Quest_updateNpcPosition(Quest* quest, int x, int y) {
    update_position_all_npc(quest->characters, x, y);
}

Character* Quest_findAnyInteraction(Quest* quest, Perso_SDL* player) {
    for(size_t i = 0 ; i < quest->characters->length ; i += 1) {
        Character* character = ArrayList_get(quest->characters, i);
        if(distance_perso(player, character->sdlPerso) < 100) {
            
            //le pnj se tourne vers le joueur
            Perso_SDL* perso = character->sdlPerso;
            int h = get_height_spritesheet(perso);
            switch (player->orientation)
            {
            case 1:
                perso->srcRect.y = h/5;
                perso->orientation = 3;
                break;
            case 2 :
                perso->srcRect.y = (h/5)*2;
                perso->orientation = 4;
                break;
            case 3 :
                perso->srcRect.y = (h/5)*3;
                perso->orientation = 1;
                break;
            case 4 :
                perso->srcRect.y = (h/5)*4;
                perso->orientation = 2;
                break;
            default:
                break;
            }
            return character;
        }
    }
    return NULL;
}

void Quest_adjustNpcPositionRelativeToMainPlayer(Quest* quest, Perso_SDL* mc) {
    for(size_t i = 0 ; i < ArrayList_length(quest->characters) ; i += 1) {
        Character* character = ArrayList_get(quest->characters, i);
        Character_adjustRelativeToMainPlayer(character, mc);
    }
}

void QuestInfo_delete(QuestInfo* infos) {
    UTF8_delete(infos->map);
    UTF8_delete(infos->name);
    if(infos->nextquest != NULL) {
        UTF8_delete(infos->nextquest);
    }
}

void Quest_delete(Quest* quest) {
    ArrayList_delete(quest->characters);
    ArrayList_delete(quest->subquests);
    free_items(quest->items);
    WDG_deleteTextArea(quest->textarea);
    WDG_deleteButton(quest->interact);
    QuestInfo_delete(&quest->infos);
    HashMap_delete(quest->sounds);
    free(quest);
}

void Subquest_delete(Subquest* subquest) {
    UTF8_delete(subquest->description);
    Bytecode_delete(subquest->initCode);
    Bytecode_delete(subquest->loopCode);
    free(subquest);
}