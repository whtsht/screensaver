#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TK_NUM,
    TK_ADD,
    TK_SUB,
    TK_MUL,
    TK_DIV,
    TK_MOD,
    TK_RPA,
    TK_LPA,
    TK_IDT,
    TK__EQ,
    TK__GO,
    TK_CGO,
    TK_CAL,
    TK_SEP,
    TK_COM,
    TK_COL,
    TK_EOF,
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
