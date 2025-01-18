#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

void PARSER_ERROR(QP_Parser* context, QP_TokenKind expected, const char* expectedValue) {
    QP_Token* token = ArrayList_get(context->tokens, context->current);
    UTF8_String* content = UTF8_empty();
    const char* ptr = context->fileContent;
    size_t n = token->line;
    size_t where = token->column;
    while(n > 1) {
        if(*ptr == '\n') {
            n -= 1;
        }
        ptr += 1;
    }
    while(*ptr != '\n') {
        UTF8_appendChar(content, *ptr);
        ptr += 1;
    }
    printf("Parsing error (%lu, %lu): '%s' (<%s>)\n", token->line, token->column, token->value->data, QP_TokenKind_repr(token->kind));
    printf("%4lu| %s\n", token->line, content->data);
    printf("    | ");
    for(size_t i = 1 ; i < where ; i += 1) {
        printf(" ");
    }
    for(size_t i = 0 ; i < token->end - token->begin ; i += 1) {
        printf("^");
    }
    printf("\n");
    size_t errMsgLength = UTF8_utf8length(QP_TokenKind_repr(expected));
    if(expectedValue != NULL) {
        errMsgLength += UTF8_utf8length(expectedValue);
    }
    size_t errMsgPos = (int) where - (int) errMsgLength / 2 < 0 ? 0 : where - errMsgLength / 2;
    printf("    | ");
    for(size_t i = 0 ; i < errMsgPos ; i += 1) {
        printf(" ");
    }
    if(expectedValue == NULL) {
        expectedValue = "";
    }
    printf("expected '%s' (%s)\n", expectedValue, QP_TokenKind_repr(expected));
    exit(-1);
}