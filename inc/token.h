#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TK_NUM,  // 0, 1, ..
    TK_ADD,  // +
    TK_SUB,  // -
    TK_MUL,  // *
    TK_DIV,  // /
    TK_MOD,  // %
    TK_RPA,  // )
    TK_LPA,  // (
    TK_IDT,  // xxx
    TK__EQ,  // =
    TK__GO,  // goto
    TK_CGO,  // cgoto
    TK_SEP,  // \n, ;
    TK_COM,  // ,
    TK_COL,  // :
    TK_EOF,  // EOF
    TK_EEQ,  // ==
    TK_NEQ,  // !=
    TK__GT,  // >
    TK__LT,  // <
    TK__GE,  // >=
    TK__LE,  // <=
    TK_DEF,  // def
    TK_END,  // end
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token* next;
    char* string;
};

typedef struct {
    char* string;
    int current;
    int size;
} Stream;

Stream* to_stream(FILE* fp);

Token* tokenize(Stream* stream);

void free_stream(Stream* stream);

int token_len(const Token* token);

Token* token_skip(Token* token, int n);

void token_skip_mut(Token** token, int n);

#endif
