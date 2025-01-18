#include "parser.h"
#include "tokens.h"
#include "errors.h"
#include "compiler.h"
#include "astitem.h"
#include "astmusic.h"
#include "../datastructures/hashmap.h"
#include "../sound.h"
#include <SDL2/SDL_mixer.h>

typedef struct {
    UTF8_String* key;
    UTF8_String* value;
} InfoPair;

void _skipEmptyLines(QP_Parser* context) {
    QP_Token* token = ArrayList_get(context->tokens, context->current);
    while(token->kind == TOK_ENDLINE) {
        context->current += 1;
        token = ArrayList_get(context->tokens, context->current);
    }
}

QP_Token* READ(QP_Parser* context, QP_TokenKind kind, const char* value) {
    QP_Token* token = ArrayList_get(context->tokens, context->current);
    if(token->kind != kind || (value != NULL && !UTF8_equalStr(token->value, value))) {
        PARSER_ERROR(context, kind, value);
    }
    context->current += 1;
    return token;
}

QP_Token* PEAK(QP_Parser* context, QP_TokenKind kind, const char* value) {
    QP_Token* token = ArrayList_get(context->tokens, context->current + 1);
    return token;
}

bool CURRENTEQ(QP_Parser* context, QP_TokenKind kind, const char* value) {
    QP_Token* token = ArrayList_get(context->tokens, context->current);
    if(token->kind == kind) {
        if(value == NULL) {
            return true;
        } else if(UTF8_equalStr(token->value, value)) {
            return true;
        }
    }
    return false;
}

bool PEAKEQ(QP_Parser* context, QP_TokenKind kind, const char* value) {
    QP_Token* token = ArrayList_get(context->tokens, context->current + 1);
    if(token->kind == kind) {
        if(value == NULL) {
            return true;
        } else if(UTF8_equalStr(token->value, value)) {
            return true;
        }
    }
    return false;
}

QP_Token* CURRENT(QP_Parser* context) {
    return ArrayList_get(context->tokens, context->current);
}

InfoPair _parseInfoPair(QP_Parser* context) {
    UTF8_String* key = UTF8_copy(READ(context, TOK_IDENTIFIER, NULL)->value);
    READ(context, TOK_SYNTAX, ":");
    UTF8_String* value = UTF8_copy(READ(context, TOK_STRING, NULL)->value);
    READ(context, TOK_ENDLINE, NULL);
    return (InfoPair) {
        .key = key,
        .value = value
    };
}

HashMap* _parseInfos(QP_Parser* context) {
    HashMap* infos = HashMap_new(UTF8_delete, UTF8_delete, UTF8_equal, UTF8_hash);
    while(CURRENT(context)->kind == TOK_IDENTIFIER) {
        InfoPair pair = _parseInfoPair(context);
        HashMap_set(infos, pair.key, pair.value);
    }
    return infos;
}

QuestInfo _parseQuestInfo(QP_Parser* context) {
    _skipEmptyLines(context);
    READ(context, TOK_SYNTAX, "@");
    READ(context, TOK_IDENTIFIER, "quest");
    int questNumber = UTF8_toInt(READ(context, TOK_NUMBER, NULL)->value);
    READ(context, TOK_ENDLINE, NULL);
    HashMap* infos = _parseInfos(context);
    // query the values
    UTF8_String* keyMap = UTF8_from("map");
    UTF8_String* keyName = UTF8_from("name");
    UTF8_String* nextquestName = UTF8_from("nextquest");
    QuestInfo result = {
        .map = UTF8_copy(HashMap_get(infos, keyMap)),
        .name = UTF8_copy(HashMap_get(infos, keyName)),
        .nextquest = HashMap_get(infos, nextquestName) != NULL ? UTF8_copy(HashMap_get(infos, nextquestName)) : NULL,
        .number = questNumber
    };
    UTF8_delete(keyMap);
    UTF8_delete(keyName);
    UTF8_delete(nextquestName);
    HashMap_delete(infos);
    return result;
}

AST_Expression* _parseStringLitteral(QP_Parser* context) {
    QP_Token* token = READ(context, TOK_STRING, NULL);
    return AST_createStringExpression(UTF8_copy(token->value));
}

AST_Expression* _parseNumberLitteral(QP_Parser* context) {
    QP_Token* token = READ(context, TOK_NUMBER, NULL);
    return AST_createNumberExpression(atoi((char*) token->value->data));
}

AST_Expression* _parseParenExpression(QP_Parser* context) {
    READ(context, TOK_SYNTAX, "(");
    AST_Expression* result = _parseExpression(context);
    READ(context, TOK_SYNTAX, ")");
    return result;
}

AST_Expression* _parseVariableExpression(QP_Parser* context) {
    QP_Token* token = READ(context, TOK_IDENTIFIER, NULL);
    if(CURRENTEQ(context, TOK_SYNTAX, "=")) {
        READ(context, TOK_SYNTAX, "=");
        AST_Expression* expression = _parseExpression(context);
        return AST_createAssignementExpression(UTF8_copy(token->value), expression);
    } else {
        return AST_createVariableExpression(UTF8_copy(token->value));
    }
}

AST_Expression* _parseAttributeExpression(QP_Parser* context) {
    QP_Token* token = READ(context, TOK_IDENTIFIER, NULL);
    if(CURRENTEQ(context, TOK_SYNTAX, "=")) {
        READ(context, TOK_SYNTAX, "=");
        AST_Expression* expression = _parseExpression(context);
        return AST_createAttributeAssignementExpression(UTF8_copy(token->value), expression);
    } else {
        return AST_createAttributeExpression(UTF8_copy(token->value));
    }
}

ArrayList* _parseFunctionArguments(QP_Parser* context) {
    ArrayList* result = ArrayList_new(AST_Expression_delete);
    READ(context, TOK_SYNTAX, "(");
    if(!CURRENTEQ(context, TOK_SYNTAX, ")")) {
        AST_Expression* expression = _parseExpression(context);
        ArrayList_append(result, expression);
        while(CURRENTEQ(context, TOK_SYNTAX, ",")) {
            READ(context, TOK_SYNTAX, ",");
            expression = _parseExpression(context);
            ArrayList_append(result, expression);
        }
    }
    READ(context, TOK_SYNTAX, ")");
    return result;
}

AST_Expression* _parseFunctionCallExpression(QP_Parser* context) {
    if(PEAKEQ(context, TOK_SYNTAX, "(")) {
        QP_Token* functionName = READ(context, TOK_IDENTIFIER, NULL);
        ArrayList* arguments = _parseFunctionArguments(context);
        return AST_createFunctionCallExpression(UTF8_copy(functionName->value), arguments);
    } else {
        return _parseVariableExpression(context);
    }
}

AST_Expression* _parseMethodCallExpression(QP_Parser* context) {
    if(PEAKEQ(context, TOK_SYNTAX, "(")) {
        QP_Token* methodName = READ(context, TOK_IDENTIFIER, NULL);
        ArrayList* arguments = _parseFunctionArguments(context);
        return AST_createMethodCallExpression(UTF8_copy(methodName->value), arguments);
    } else {
        return _parseAttributeExpression(context);
    }
}

AST_Expression* _parseFinalExpression(QP_Parser* context) {
    AST_Expression* result;
    if(CURRENTEQ(context, TOK_STRING, NULL)) {
        result = _parseStringLitteral(context);
    } else if(CURRENTEQ(context, TOK_NUMBER, NULL)) {
        result = _parseNumberLitteral(context);
    } else if(CURRENTEQ(context, TOK_SYNTAX, "(")) {
        result = _parseParenExpression(context);
    } else if(CURRENTEQ(context, TOK_IDENTIFIER, NULL)) {
        result = _parseFunctionCallExpression(context);
    } else {
        PARSER_ERROR(context, TOK_STRING, NULL);
    }
    return result; // unreachable
}

AST_Expression* _parseGetattributeExpression(QP_Parser* context) {
    AST_Expression* expr = _parseFinalExpression(context);
    if(CURRENTEQ(context, TOK_SYNTAX, ".")) {
        ArrayList* expressions = ArrayList_new(AST_Expression_delete);
        ArrayList_append(expressions, expr);
        while(CURRENTEQ(context, TOK_SYNTAX, ".")) {
            READ(context, TOK_SYNTAX, ".");
            AST_Expression* expression = _parseMethodCallExpression(context);
            ArrayList_append(expressions, expression);
        }
        return AST_createGetattributeExpression(expressions);
    } else {
        return expr;
    }
}

AST_Expression* _parseMulDivExpression(QP_Parser* context) {
    AST_Expression* left = _parseGetattributeExpression(context);
    if(CURRENTEQ(context, TOK_SYNTAX, "*")) {
        READ(context, TOK_SYNTAX, "*");
        AST_Expression* right = _parseAddSubExpression(context);
        return AST_createBinaryExpression(AST_MULTIPLICATION, left, right);
    } else {
        return left;
    }
}

AST_Expression* _parseAddSubExpression(QP_Parser* context) {
    AST_Expression* left = _parseMulDivExpression(context);
    if(CURRENTEQ(context, TOK_SYNTAX, "+")) {
        READ(context, TOK_SYNTAX, "+");
        AST_Expression* right = _parseAddSubExpression(context);
        return AST_createBinaryExpression(AST_ADDITION, left, right);
    } else if(CURRENTEQ(context, TOK_SYNTAX, "-")) {
        READ(context, TOK_SYNTAX, "-");
        AST_Expression* right = _parseAddSubExpression(context);
        return AST_createBinaryExpression(AST_SUBSTRACTION, left, right);
    } else {
        return left;
    }
}

AST_Expression* _parseEqNEqExpression(QP_Parser* context) {
    AST_Expression* left = _parseAddSubExpression(context);
    if(CURRENTEQ(context, TOK_SYNTAX, "==")) {
        READ(context, TOK_SYNTAX, "==");
        AST_Expression* right = _parseEqNEqExpression(context);
        return AST_createBinaryExpression(AST_EQUAL, left, right);
    } else if(CURRENTEQ(context, TOK_SYNTAX, "!=")) {
        READ(context, TOK_SYNTAX, "!=");
        AST_Expression* right = _parseEqNEqExpression(context);
        return AST_createBinaryExpression(AST_NOT_EQUAL, left, right);
    } else if(CURRENTEQ(context, TOK_SYNTAX, ">=")) {
        READ(context, TOK_SYNTAX, ">=");
        AST_Expression* right = _parseEqNEqExpression(context);
        return AST_createBinaryExpression(AST_GREATER_OR_EQUAL, left, right);
    } else if(CURRENTEQ(context, TOK_SYNTAX, "<=")) {
        READ(context, TOK_SYNTAX, "<=");
        AST_Expression* right = _parseEqNEqExpression(context);
        return AST_createBinaryExpression(AST_LOWER_OR_EQUAL, left, right);
    } else if(CURRENTEQ(context, TOK_SYNTAX, ">")) {
        READ(context, TOK_SYNTAX, ">");
        AST_Expression* right = _parseEqNEqExpression(context);
        return AST_createBinaryExpression(AST_GREATER_THAN, left, right);
    } else if(CURRENTEQ(context, TOK_SYNTAX, ">=")) {
        READ(context, TOK_SYNTAX, "<");
        AST_Expression* right = _parseEqNEqExpression(context);
        return AST_createBinaryExpression(AST_LOWER_THAN, left, right);
    } else {
        return left;
    }
}

AST_Expression* _parseExpression(QP_Parser* context) {
    return _parseEqNEqExpression(context);
}

AST_Statement* _parseExpressionStatement(QP_Parser* context) {
    AST_Expression* expression = _parseExpression(context);
    return AST_createExpressionStatement(expression);
}

ArrayList* _parseElseInstruction(QP_Parser* context) {
    READ(context, TOK_IDENTIFIER, "else");
    if(CURRENTEQ(context, TOK_IDENTIFIER, "if")) {
        ArrayList* instructions = ArrayList_new(AST_Statement_delete);
        ArrayList_append(instructions, _parseConditionalStatement(context));
        return instructions;
    } else {
        return _parseInstructionsBlock(context);
    }
}

AST_Statement* _parseConditionalStatement(QP_Parser* context) {
    READ(context, TOK_IDENTIFIER, "if");
    AST_Expression* condition = _parseExpression(context);
    _skipEmptyLines(context);
    ArrayList* trueBlock = _parseInstructionsBlock(context);
    ArrayList* falseBlock = NULL;
    if(CURRENTEQ(context, TOK_IDENTIFIER, "else")) {
        falseBlock = _parseElseInstruction(context);
    } else {
        falseBlock = ArrayList_new(AST_Statement_delete);
    }
    return AST_createIfStatement(condition, trueBlock, falseBlock);
}

AST_Statement*_parseInstruction(QP_Parser* context) {
    if(CURRENTEQ(context, TOK_IDENTIFIER, "if")) {
        return _parseConditionalStatement(context);
    } else {
        return _parseExpressionStatement(context);
    }
}

ArrayList* _parseInstructionsBlock(QP_Parser* context) {
    READ(context, TOK_SYNTAX, "{");
    _skipEmptyLines(context);
    ArrayList* instructions = ArrayList_new(AST_Statement_delete);
    while(!CURRENTEQ(context, TOK_SYNTAX, "}")) {
        AST_Statement* instruction = _parseInstruction(context);
        ArrayList_append(instructions, instruction);
        _skipEmptyLines(context);
    }
    READ(context, TOK_SYNTAX, "}");
    _skipEmptyLines(context);
    return instructions;
}

Character* _parseCharacter(QP_Parser* context) {
    READ(context, TOK_SYNTAX, "@");
    READ(context, TOK_IDENTIFIER, "character");
    UTF8_String* name = UTF8_copy(READ(context, TOK_STRING, NULL)->value);
    READ(context, TOK_ENDLINE, NULL);
    HashMap* infos = _parseInfos(context);
    // read the associated instruction and compile it in bytecode
    ArrayList* instructions = _parseInstructionsBlock(context);
    Bytecode* bytecode = compile(instructions);
    // Bytecode_print(bytecode);
    // free the now useless instructions
    ArrayList_delete(instructions);
    // construct the character
    UTF8_String* skinName = UTF8_from("skin");
    UTF8_String* spawnxStr = UTF8_from("spawnx");
    UTF8_String* spawnyStr = UTF8_from("spawny");
    UTF8_String* orientationStr = UTF8_from("orientation");
    UTF8_String* skin = UTF8_copy(HashMap_get(infos, skinName));
    int spawnx = atoi((char*) ((UTF8_String*) HashMap_get(infos, spawnxStr))->data);
    int spawny = atoi((char*) ((UTF8_String*) HashMap_get(infos, spawnyStr))->data);
    int orientation = atoi((char*) ((UTF8_String*) HashMap_get(infos, orientationStr))->data);
    // free the memory
    UTF8_delete(skinName);
    UTF8_delete(spawnxStr);
    UTF8_delete(spawnyStr);
    UTF8_delete(orientationStr);
    HashMap_delete(infos);
    // return the new character
    Character* result = Character_new(name, skin, spawnx, spawny, orientation, bytecode, context->renderer);
    return result;
}

AST_Item* _parseItem(QP_Parser* context) {
    READ(context, TOK_SYNTAX, "@");
    READ(context, TOK_IDENTIFIER, "item");
    UTF8_String* name = UTF8_copy(READ(context, TOK_STRING, NULL)->value);
    READ(context, TOK_ENDLINE, NULL);
    HashMap* infos = _parseInfos(context);
    // construct the item
    UTF8_String* image = UTF8_from("image");
    UTF8_String* foundStr = UTF8_from("found");
    UTF8_String* imagePath = UTF8_copy(HashMap_get(infos, image));
    UTF8_String* found = HashMap_get(infos, foundStr);
    bool itemFound = false;
    if(found != NULL && UTF8_equalStr(found, "true")) {
        itemFound = true;
    }
    // free the memory
    UTF8_delete(image);
    UTF8_delete(foundStr);
    HashMap_delete(infos);
    // return the new character
    return AST_Item_new(name, imagePath, itemFound);
}

Subquest* _parseSubquest(QP_Parser* context) {
    READ(context, TOK_SYNTAX, "@");
    READ(context, TOK_IDENTIFIER, "subquest");
    int number = atoi((char*) READ(context, TOK_NUMBER, NULL)->value->data);
    READ(context, TOK_ENDLINE, NULL);
    HashMap* infos = _parseInfos(context);
    // parse the code blocks
    READ(context, TOK_SYNTAX, "#");
    READ(context, TOK_IDENTIFIER, "init");
    ArrayList* initInstructions = _parseInstructionsBlock(context);
    _skipEmptyLines(context);
    READ(context, TOK_SYNTAX, "#");
    READ(context, TOK_IDENTIFIER, "loop");
    ArrayList* loopInstructions = _parseInstructionsBlock(context);
    // transform the instructions into bytecode
    Bytecode* initCode = compile(initInstructions);
    Bytecode* loopCode = compile(loopInstructions);
    // construct the item
    UTF8_String* descriptionName = UTF8_from("description");
    UTF8_String* description = UTF8_copy(HashMap_get(infos, descriptionName));
    // free the memory
    ArrayList_delete(initInstructions);
    ArrayList_delete(loopInstructions);
    UTF8_delete(descriptionName);
    HashMap_delete(infos);
    // return the new character
    return Subquest_new(number, GOAL_FIND_OBJECT, description, initCode, loopCode);
}

AST_Music* _parseMusic(QP_Parser* context) {
    READ(context, TOK_SYNTAX, "@");
    READ(context, TOK_IDENTIFIER, "sound");
    UTF8_String* musicName = UTF8_copy(READ(context, TOK_STRING, NULL)->value);
    READ(context, TOK_ENDLINE, NULL);
    HashMap* infos = _parseInfos(context);
    // construct the item
    UTF8_String* key = UTF8_from("sound");
    UTF8_String* path = UTF8_copy(HashMap_get(infos, key));
    // free the memory
    HashMap_delete(infos);
    // return the new music
    Mix_Chunk* music = load_sound(UTF8_str(path));
    UTF8_delete(path);
    UTF8_delete(key);
    return AST_Music_new(musicName, music);
}

void _parseCharactersAndItems(QP_Parser* context, ArrayList* characters, ArrayList* subquests, ArrayList* items, HashMap* sounds) {
    _skipEmptyLines(context);
    while(CURRENTEQ(context, TOK_SYNTAX, "@")) {
        if(PEAKEQ(context, TOK_IDENTIFIER, "character")) {
            Character* character = _parseCharacter(context);
            ArrayList_append(characters, character);
        } else if(PEAKEQ(context, TOK_IDENTIFIER, "item")) {
            AST_Item* item = _parseItem(context);
            ArrayList_append(items, item);
        } else if(PEAKEQ(context, TOK_IDENTIFIER, "subquest")) {
            Subquest* subquest = _parseSubquest(context);
            ArrayList_append(subquests, subquest);
        } else if(PEAKEQ(context, TOK_IDENTIFIER, "sound")) {
            AST_Music* music = _parseMusic(context);
            HashMap_set(sounds, music->name, music->music);
            AST_Music_delete(music);
        }
        _skipEmptyLines(context);
    }
}

Quest* parseQuest(const char* content, SDL_Renderer* renderer) {
    ArrayList* tokens = tokenize(content);
    QP_Parser context = {
        .fileContent = content,
        .current = 0,
        .tokens = tokens,
        .renderer = renderer
    };
    QuestInfo infos = _parseQuestInfo(&context);
    ArrayList* characters = ArrayList_new(Character_delete);
    ArrayList* parsedItems = ArrayList_new(AST_Item_delete);
    ArrayList* subquests = ArrayList_new(Subquest_delete);
    HashMap* sounds = HashMap_new(UTF8_delete, Mix_FreeChunk, UTF8_equal, UTF8_hash);
    _parseCharactersAndItems(&context, characters, subquests, parsedItems, sounds);
    Items* items = init_items(renderer, parsedItems);
    ArrayList_delete(tokens);
    ArrayList_delete(parsedItems);
    return Quest_new(infos, characters, subquests, items, sounds);
}