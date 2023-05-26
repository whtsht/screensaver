#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TK_NUM,
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
void forward(int distance);
