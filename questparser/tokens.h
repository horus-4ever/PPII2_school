#ifndef QUESTPARSER_TOKENS
#define QUESTPARSER_TOKENS

#include "../utf8strings/utf8strings.h"
#include "../datastructures/array.h"

/**
 * Enumeration of QP token kinds.
 */
typedef enum {
    TOK_STRING, /**< String token kind. */
    TOK_IDENTIFIER, /**< Identifier token kind. */
    TOK_SYNTAX, /**< Syntax token kind. */
    TOK_NUMBER, /**< Number token kind. */
    TOK_ENDLINE, /**< Endline token kind. */
    TOK_ENDFILE /**< End of file token kind. */
} QP_TokenKind;

/**
 * Structure representing a token.
 */
typedef struct {
    QP_TokenKind kind; /**< Kind of the token. */
    UTF8_String* value; /**< Value of the token. */
    size_t begin; /**< Beginning position of the token in the file content. */
    size_t end; /**< Ending position of the token in the file content. */
    size_t line; /**< Line number where the token occurs. */
    size_t column; /**< Column number where the token occurs. */
} QP_Token;

/**
 * Structure representing the QP Tokenizer.
 */
typedef struct QP_Tokenizer {
    const char* fileContent; /**< Pointer to the content of the file being tokenized. */
    size_t current; /**< Current position in the file content. */
    size_t line; /**< Current line number. */
    size_t column; /**< Current column number. */
} QP_Tokenizer;

/**
 * Tokenizes the input string and returns an ArrayList of tokens.
 * @param str A pointer to the input string.
 * @return A pointer to the ArrayList containing the tokens.
 */
ArrayList* tokenize(const char* str);

/**
 * Creates a new token.
 * @param kind The kind of the token.
 * @param begin The beginning position of the token.
 * @param end The ending position of the token.
 * @param line The line number where the token occurs.
 * @param column The column number where the token occurs.
 * @param value The value of the token.
 * @return A pointer to the newly created token.
 */
QP_Token* QP_Token_new(QP_TokenKind kind, size_t begin, size_t end, size_t line, size_t column, UTF8_String* value);

/**
 * Initializes a token.
 * @param token A pointer to the token to be initialized.
 * @param kind The kind of the token.
 * @param begin The beginning position of the token.
 * @param end The ending position of the token.
 * @param line The line number where the token occurs.
 * @param column The column number where the token occurs.
 * @param value The value of the token.
 */
void QP_Token_init(QP_Token* token, QP_TokenKind kind, size_t begin, size_t end, size_t line, size_t column, UTF8_String* value);

/**
 * Deletes a token and frees associated resources.
 * @param token A pointer to the token to be deleted.
 */
void QP_Token_delete(QP_Token* token);

/**
 * Prints the token.
 * @param token A pointer to the token to be printed.
 */
void QP_Token_print(QP_Token* token);

/**
 * Gets a string representation of a token kind.
 * @param kind The token kind.
 * @return A string representation of the token kind.
 */
const char* QP_TokenKind_repr(QP_TokenKind kind);

#endif