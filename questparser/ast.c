#include "ast.h"

AST_Expression* AST_createBinaryExpression(AST_BinaryOperatorKind kind, AST_Expression* left, AST_Expression* right) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_BINARY_EXPR;
    result->binaryExpr = (AST_BinaryExpression) {
        .left = left,
        .right = right,
        .operator = kind
    };
    return result;
}

AST_Expression* AST_createStringExpression(UTF8_String* string) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_LITTERAL_EXPR;
    result->litteral = (AST_Litteral) {
        .kind = AST_STRING,
        .string = string
    };
    return result;
}

AST_Expression* AST_createNumberExpression(int64_t value) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_LITTERAL_EXPR;
    result->litteral = (AST_Litteral) {
        .kind = AST_NUMBER,
        .number = value
    };
    return result;
}

AST_Expression* AST_createVariableExpression(UTF8_String* varName) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_LITTERAL_EXPR;
    result->litteral = (AST_Litteral) {
        .kind = AST_VARIABLE,
        .name = varName
    };
    return result;
}

AST_Expression* AST_createAttributeExpression(UTF8_String* varName) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_LITTERAL_EXPR;
    result->litteral = (AST_Litteral) {
        .kind = AST_ATTRIBUTE,
        .name = varName
    };
    return result;
}

AST_Expression* AST_createFunctionCallExpression(UTF8_String* funcName, ArrayList* arguments) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_FUNCTION_CALL_EXPR;
    result->functionCall = (AST_FunctionCall) {
        .funcName = funcName,
        .arguments = arguments
    };
    return result;
}

AST_Expression* AST_createMethodCallExpression(UTF8_String* funcName, ArrayList* arguments) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_METHOD_CALL_EXPR;
    result->methodCall = (AST_FunctionCall) {
        .funcName = funcName,
        .arguments = arguments
    };
    return result;
}

AST_Statement* AST_createIfStatement(AST_Expression* condition, ArrayList* trueBlock, ArrayList* falseBlock) {
    AST_Statement* result = malloc(sizeof(AST_Statement));
    result->kind = AST_CONDITIONAL_STATEMENT;
    result->condition.expr = condition;
    result->condition.trueBlock = trueBlock;
    result->condition.falseBlock = falseBlock;
    return result;
}

AST_Statement* AST_createExpressionStatement(AST_Expression* expression) {
    AST_Statement* result = malloc(sizeof(AST_Statement));
    result->kind = AST_EXPRESSION_STATEMENT;
    result->expression = expression;
    return result;
}

AST_Expression* AST_createAssignementExpression(UTF8_String* varName, AST_Expression* expr) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_ASSIGNEMENT_EXPR;
    result->assignement.expression = expr;
    result->assignement.name = varName;
    return result;
}

AST_Expression* AST_createAttributeAssignementExpression(UTF8_String* name, AST_Expression* expr) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_SETATTRIBUTE_EXPR;
    result->setAttribute.expression = expr;
    result->setAttribute.name = name;
    return result;
}

AST_Expression* AST_createGetattributeExpression(ArrayList* expressions) {
    AST_Expression* result = malloc(sizeof(AST_Expression));
    result->kind = AST_GETATTRIBUTE_EXPR;
    result->chain = expressions;
    return result;
}

void _AST_Expression_deleteLitteral(AST_Expression* expression) {
    switch (expression->litteral.kind) {
    case AST_VARIABLE:
        UTF8_delete(expression->litteral.name);
        break;
    case AST_STRING:
        UTF8_delete(expression->litteral.string);
        break;
    case AST_ATTRIBUTE:
        UTF8_delete(expression->litteral.name);
        break;
    default:
        break;
    }
}

void _AST_Expression_deleteBinary(AST_Expression* expression) {
    AST_Expression_delete(expression->binaryExpr.left);
    AST_Expression_delete(expression->binaryExpr.right);
}

void _AST_Expression_deleteFunctionCall(AST_Expression* expression) {
    UTF8_delete(expression->functionCall.funcName);
    ArrayList_delete(expression->functionCall.arguments);
}

void _AST_Expression_deleteGetattributeExpression(AST_Expression* expression) {
    ArrayList_delete(expression->chain);
}

void _AST_Expression_deleteMethodCallExpression(AST_Expression* expression) {
    ArrayList_delete(expression->methodCall.arguments);
    UTF8_delete(expression->methodCall.funcName);
}

void _AST_Expression_deleteAssignementExpression(AST_Expression* expression) {
    AST_Expression_delete(expression->assignement.expression);
    UTF8_delete(expression->assignement.name);
}

void _AST_Expression_deleteSetattributeExpression(AST_Expression* expression) {
    AST_Expression_delete(expression->setAttribute.expression);
    UTF8_delete(expression->setAttribute.name);
}

void AST_Expression_delete(AST_Expression* expression) {
    switch (expression->kind) {
    case AST_LITTERAL_EXPR:
        _AST_Expression_deleteLitteral(expression);
        break;
    case AST_BINARY_EXPR:
        _AST_Expression_deleteBinary(expression);
        break;
    case AST_FUNCTION_CALL_EXPR:
        _AST_Expression_deleteFunctionCall(expression);
        break;
    case AST_GETATTRIBUTE_EXPR:
        _AST_Expression_deleteGetattributeExpression(expression);
        break;
    case AST_METHOD_CALL_EXPR:
        _AST_Expression_deleteMethodCallExpression(expression);
        break;
    case AST_ASSIGNEMENT_EXPR:
        _AST_Expression_deleteAssignementExpression(expression);
        break;
    case AST_SETATTRIBUTE_EXPR:
        _AST_Expression_deleteSetattributeExpression(expression);
        break;
    default:
        break;
    }
    free(expression);
}

void _AST_Statement_deleteCondition(AST_Statement* instruction) {
    AST_Expression_delete(instruction->condition.expr);
    ArrayList_delete(instruction->condition.trueBlock);
    ArrayList_delete(instruction->condition.falseBlock);
}

void AST_Statement_delete(AST_Statement* instruction) {
    switch (instruction->kind) {
    case AST_EXPRESSION_STATEMENT:
        AST_Expression_delete(instruction->expression);
        break;
    case AST_CONDITIONAL_STATEMENT:
        _AST_Statement_deleteCondition(instruction);
        break;
    default:
        break;
    }
    free(instruction);
}