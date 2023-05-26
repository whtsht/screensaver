#include "../inc/lang.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

Stream* to_stream(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    char* value = calloc(1, size);
    rewind(fp);

    fread(value, 1, size, fp);

    Stream* stream = calloc(1, sizeof(Stream));
    stream->string = value;
    stream->current = 0;
    stream->size = size;

    return stream;
}

int stream_iseof(Stream* stream) {
    return stream->current >= stream->size;
}

char stream_peek(Stream* stream) {
    if (stream_iseof(stream)) {
        return EOF;
    }
    return stream->string[stream->current];
}

void stream_next(Stream* stream) {
    stream->current += 1;
}

Token* new_token(Token* current, TokenKind kind, char* string) {
    Token* token = (Token*)calloc(1, sizeof(Token));
    token->kind = kind;
    token->string = string;
    token->next = NULL;
    current->next = token;
    return token;
}

char* parse_digit(Stream* stream) {
    char* str = calloc(20, sizeof(char));

    int i = 0;
    while (1) {
        char c = stream_peek(stream);
        if (isdigit(c)) {
            str[i] = c;
            i += 1;
            stream_next(stream);
        } else {
            if (i == 0) {
                free(str);
                return NULL;
            } else {
                str[i] = '\0';
                return str;
            }
        }
    }
    return str;
}

Token* tokenize(Stream* stream) {
    Token head;
    head.next = NULL;
    Token* cur = &head;

    while (!stream_iseof(stream)) {
        if (stream_peek(stream) == ' ' || stream_peek(stream) == '\n') {
            stream_next(stream);
            continue;
        }
        char* str = parse_digit(stream);
        if (str != NULL) {
            cur = new_token(cur, TK_NUM, str);
        }
    }

    new_token(cur, TK_EOF, "");
    return head.next;
}
