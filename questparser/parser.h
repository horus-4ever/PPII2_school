#ifndef QUESTPARSER_PARSER
#define QUESTPARSER_PARSER

#include "../quest/quest.h"
#include "ast.h"
#include "../datastructures/hashmap.h"
#include "../datastructures/array.h"
#include <stddef.h>

/**
 * Structure representing the QP Parser.
 */
typedef struct {
    const char* fileContent; /**< Pointer to the content of the file being parsed. */
    size_t current; /**< Current position in the file content. */
    ArrayList* tokens; /**< ArrayList of tokens generated during parsing. */
    SDL_Renderer* renderer; /**< Pointer to the SDL_Renderer used for rendering. */
} QP_Parser;

/**
 * Parses the content of a quest file and generates a Quest object.
 * @param content A pointer to the content of the quest file.
 * @param renderer A pointer to the SDL_Renderer used for rendering.
 * @return A pointer to the generated Quest object.
 */
Quest* parseQuest(const char* content, SDL_Renderer* renderer);

/*
PARSER
*/

HashMap* _parseInfos(QP_Parser* context);

QuestInfo _parseQuestInfo(QP_Parser* context);

AST_Expression* _parseStringLitteral(QP_Parser* context);

AST_Expression* _parseParenExpression(QP_Parser* context);

AST_Expression* _parseFinalExpression(QP_Parser* context);

AST_Expression* _parseAddSubExpression(QP_Parser* context);

AST_Expression* _parseExpression(QP_Parser* context);

AST_Statement* _parseLineCommand(QP_Parser* context);

AST_Statement* _parseAskCommand(QP_Parser* context);

AST_Statement* _parseConditionalStatement(QP_Parser* context);

AST_Statement* _parseExpressionStatement(QP_Parser* context);

AST_Statement*_parseInstruction(QP_Parser* context);

ArrayList* _parseInstructionsBlock(QP_Parser* context);

Character* _parseCharacter(QP_Parser* context);

ArrayList* _parseCharacters(QP_Parser* context);

#endif