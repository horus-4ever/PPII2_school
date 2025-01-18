#include "tokens.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

QP_Token* QP_Token_new(QP_TokenKind kind, size_t begin, size_t end, size_t line, size_t column, UTF8_String* value) {
    QP_Token* result = malloc(sizeof(QP_Token));
    QP_Token_init(result, kind, begin, end, line, column, value);
    return result;
}

void QP_Token_init(QP_Token* token, QP_TokenKind kind, size_t begin, size_t end, size_t line, size_t column, UTF8_String* value) {
    token->begin = begin;
    token->end = end;
    token->kind = kind;
    token->value = value;
    token->line = line;
    token->column = column;
}

void QP_Token_delete(QP_Token* token) {
    UTF8_delete(token->value);
    free(token);
}

static char CURRENT(QP_Tokenizer* context) {
    return context->fileContent[context->current];
}

bool IS_EOF(QP_Tokenizer* context) {
    return context->current >= strlen(context->fileContent);
}

void NEXT(QP_Tokenizer* context) {
    if(!IS_EOF(context)) {
        if(CURRENT(context) == '\n') {
            context->line += 1;
            context->column = 1;
        } else {
            context->column += 1;
        }
    }
    context->current += 1;
}

QP_Token* _readIdentifier(QP_Tokenizer* context) {
    size_t begin = context->current;
    size_t line = context->line;
    size_t column = context->column;
    UTF8_String* value = UTF8_empty();
    while(isalnum(CURRENT(context))) {
        UTF8_appendChar(value, CURRENT(context));
        NEXT(context);
    }
    QP_Token* result = QP_Token_new(TOK_IDENTIFIER, begin, context->current, line, column, value);
    return result;
}

QP_Token* _readNumber(QP_Tokenizer* context) {
    size_t begin = context->current;
    size_t line = context->line;
    size_t column = context->column;
    UTF8_String* value = UTF8_empty();
    while(isdigit(CURRENT(context))) {
        UTF8_appendChar(value, CURRENT(context));
        NEXT(context);
    }
    QP_Token* result = QP_Token_new(TOK_NUMBER, begin, context->current, line, column, value);
    return result;}

QP_Token* _readString(QP_Tokenizer* context) {
    size_t begin = context->current;
    size_t line = context->line;
    size_t column = context->column;
    NEXT(context);
    UTF8_String* value = UTF8_empty();
    while(CURRENT(context) != '"') {
        UTF8_appendChar(value, CURRENT(context));
        NEXT(context);
    }
    NEXT(context);
    QP_Token* result = QP_Token_new(TOK_STRING, begin, context->current, line, column, value);
    return result;
}

QP_Token* _readSyntax(QP_Tokenizer* context) {
    size_t begin = context->current;
    size_t line = context->line;
    size_t column = context->column;
    UTF8_String* value = UTF8_empty();
    do {
        UTF8_appendChar(value, CURRENT(context));
        NEXT(context);
    } while(!IS_EOF(context) && !isspace(CURRENT(context)) && !isalnum(CURRENT(context)) && (CURRENT(context) != '"') && (CURRENT(context) != ')') && (CURRENT(context) != '.') && (CURRENT(context) != '('));
    QP_Token* result = QP_Token_new(TOK_SYNTAX, begin, context->current, line, column, value);
    return result;
}

QP_Token* _readNewline(QP_Tokenizer* context) {
    size_t begin = context->current;
    size_t line = context->line;
    size_t column = context->column;
    UTF8_String* value = UTF8_empty();
    UTF8_appendChar(value, CURRENT(context));
    NEXT(context);
    QP_Token* result = QP_Token_new(TOK_ENDLINE, begin, context->current, line, column, value);
    return result;
}

QP_Token* _endoffile(QP_Tokenizer* context) {
    size_t begin = context->current;
    size_t line = context->line;
    size_t column = context->column;
    UTF8_String* value = UTF8_from("EOF");
    QP_Token* result = QP_Token_new(TOK_ENDFILE, begin, context->current, line, column, value);
    return result;
}

ArrayList* tokenize(const char* str) {
    QP_Tokenizer context = {
        .fileContent = str,
        .current = 0,
        .line = 1,
        .column = 1
    };
    ArrayList* result = ArrayList_new(QP_Token_delete);
    while(CURRENT(&context) != '\0') {
        if(CURRENT(&context) == '\n') {
            QP_Token* token = _readNewline(&context);
            ArrayList_append(result, token);
        } else if(CURRENT(&context) == '"') {
            QP_Token* token = _readString(&context);
            ArrayList_append(result, token);
        } else if(isalpha(CURRENT(&context))) {
            QP_Token* token = _readIdentifier(&context);
            ArrayList_append(result, token);
        } else if(isdigit(CURRENT(&context))) {
            QP_Token* token = _readNumber(&context);
            ArrayList_append(result, token);
        } else if(isspace(CURRENT(&context))) {
            NEXT(&context);
        } else {
            QP_Token* token = _readSyntax(&context);
            ArrayList_append(result, token);
        }
    }
    ArrayList_append(result, _endoffile(&context));
    return result;
}

const char* QP_TokenKind_repr(QP_TokenKind kind) {
    switch (kind) {
    case TOK_STRING:
        return "TOK_STRING";
    case TOK_IDENTIFIER:
        return "TOK_IDENTIFIER";
    case TOK_SYNTAX:
        return "TOK_SYNTAX";
    case TOK_NUMBER:
        return "TOK_NUMBER";
    case TOK_ENDFILE:
        return "TOK_ENDFILE";
    case TOK_ENDLINE:
        return "TOK_ENDLINE";
    default:
        break;
    }
    return NULL; // unreachable
}

void QP_Token_print(QP_Token* token) {
    printf("<%s: %s (%lu %lu)>\n", QP_TokenKind_repr(token->kind), token->value->data, token->line, token->column);
}