#ifndef QP_ERROR
#define QP_ERROR

#include "tokens.h"
#include "parser.h"

/**
 * Reports a parser error.
 * @param context A pointer to the QP_Parser context.
 * @param expected The expected token kind.
 * @param expectedValue The expected token value.
 */
void PARSER_ERROR(QP_Parser* context, QP_TokenKind expected, const char* expectedValue);

#endif