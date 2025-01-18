#ifndef AST_H
#define AST_H

#include "../datastructures/hashmap.h"
#include "../datastructures/array.h"
#include "../utf8strings/utf8strings.h"
#include <stddef.h>

struct AST_Expression;

typedef enum {
    AST_VARIABLE,
    AST_ATTRIBUTE,
    AST_STRING,
    AST_NUMBER
} AST_LitteralKind;

typedef struct {
    AST_LitteralKind kind;
    union {
        UTF8_String* string;
        int64_t number;
        UTF8_String* name;
    };
} AST_Litteral;

typedef enum {
    AST_ADDITION,
    AST_SUBSTRACTION,
    AST_MULTIPLICATION,
    AST_EQUAL,
    AST_NOT_EQUAL,
    AST_GREATER_OR_EQUAL,
    AST_LOWER_OR_EQUAL,
    AST_GREATER_THAN,
    AST_LOWER_THAN
} AST_BinaryOperatorKind;

typedef struct {
    struct AST_Expression* right;
    struct AST_Expression* left;
    AST_BinaryOperatorKind operator;
} AST_BinaryExpression;

typedef struct {
    UTF8_String* funcName;
    ArrayList* arguments;
} AST_FunctionCall;

typedef struct {
    UTF8_String* name;
    struct AST_Expression* expression;
} AST_Assignement;

typedef struct {
    UTF8_String* name;
    struct AST_Expression* expression;
} AST_SetAttribute;

typedef enum {
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_FUNCTION_CALL_EXPR,
    AST_METHOD_CALL_EXPR,
    AST_LITTERAL_EXPR,
    AST_GETATTRIBUTE_EXPR,
    AST_ASSIGNEMENT_EXPR,
    AST_SETATTRIBUTE_EXPR
} AST_ExpressionKind;

typedef struct AST_Expression {
    AST_ExpressionKind kind;
    union {
        AST_BinaryExpression binaryExpr;
        AST_Litteral litteral;
        AST_FunctionCall functionCall;
        AST_FunctionCall methodCall;
        ArrayList* chain;
        AST_Assignement assignement;
        AST_SetAttribute setAttribute;
    };
} AST_Expression;

typedef enum {
    AST_EXPRESSION_STATEMENT,
    AST_CONDITIONAL_STATEMENT
} AST_StatementKind;

typedef struct {
    AST_StatementKind kind;
    union {
        AST_Expression* expression;
        struct {
            AST_Expression* expr;
            ArrayList* trueBlock;
            ArrayList* falseBlock;
        } condition;
    };
} AST_Statement;

AST_Expression* AST_createBinaryExpression(AST_BinaryOperatorKind kind, AST_Expression* left, AST_Expression* right);
AST_Expression* AST_createStringExpression(UTF8_String* string);
AST_Expression* AST_createNumberExpression(int64_t value);
AST_Expression* AST_createVariableExpression(UTF8_String* string);
AST_Expression* AST_createAttributeExpression(UTF8_String* string);
AST_Expression* AST_createFunctionCallExpression(UTF8_String* funcName, ArrayList* arguments);
AST_Expression* AST_createMethodCallExpression(UTF8_String* funcName, ArrayList* arguments);
AST_Expression* AST_createGetattributeExpression(ArrayList* expressions);
AST_Statement* AST_createIfStatement(AST_Expression* condition, ArrayList* trueBlock, ArrayList* falseBlock);
AST_Statement* AST_createExpressionStatement(AST_Expression* expression);
AST_Expression* AST_createAssignementExpression(UTF8_String* varName, AST_Expression* expr);
AST_Expression* AST_createAttributeAssignementExpression(UTF8_String* name, AST_Expression* expr);

void AST_Expression_delete(AST_Expression* expression);
void AST_Statement_delete(AST_Statement* expression);


#endif