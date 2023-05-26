#include "../inc/token.h"

#include <ctype.h>
#include <string.h>

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

void free_stream(Stream* stream) {
    free(stream->string);
}

int stream_iseof(const Stream* stream) {
    return stream->current >= stream->size;
}

char stream_peek(const Stream* stream) {
    if (stream_iseof(stream)) {
        return EOF;
    }
    return stream->string[stream->current];
}

void stream_back(Stream* stream, int value) {
    stream->current -= value;
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
}

char* RESERVED_WORD[] = {"goto", "forward"};
int RESERVED_WORD_LENGTH = sizeof(RESERVED_WORD) / sizeof(char*);

int is_reserved(char* target) {
    for (int i = 0; i < RESERVED_WORD_LENGTH; i++) {
        if (strcmp(RESERVED_WORD[i], target) == 0) {
            return 1;
        }
    }
    return 0;
}

char* parse_ident(Stream* stream) {
    char* str = calloc(20, sizeof(char));
    int i = 0;
    while (1) {
        char c = stream_peek(stream);
        if (isalpha(c)) {
            str[i] = c;
            i += 1;
            stream_next(stream);
        } else {
            if (i == 0) {
                free(str);
                return NULL;
            }
            str[i] = '\0';
            if (is_reserved(str)) {
                free(str);
                return NULL;
            }
            return str;
        }
    }
}

Token* tokenize(Stream* stream) {
    Token head;
    head.next = NULL;
    Token* cur = &head;

    while (!stream_iseof(stream)) {
        if (stream_peek(stream) == ' ') {
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '\n') {
            cur = new_token(cur, TK_SEP, "\n");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == ';') {
            cur = new_token(cur, TK_SEP, ";");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '+') {
            cur = new_token(cur, TK_ADD, "+");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '-') {
            cur = new_token(cur, TK_SUB, "-");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '*') {
            cur = new_token(cur, TK_MUL, "*");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '/') {
            cur = new_token(cur, TK_DIV, "/");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '%') {
            cur = new_token(cur, TK_MOD, "%");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == '(') {
            cur = new_token(cur, TK_LPA, "(");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == ')') {
            cur = new_token(cur, TK_RPA, ")");
            stream_next(stream);
            continue;
        }

        char* digit = parse_digit(stream);
        if (digit != NULL) {
            cur = new_token(cur, TK_NUM, digit);
            continue;
        }

        char* ident = parse_ident(stream);
        if (ident != NULL) {
            cur = new_token(cur, TK_NUM, ident);
            continue;
        }

        fprintf(stderr, "failed to tokenize: (%c)", stream_peek(stream));
        exit(0);
    }

    new_token(cur, TK_EOF, "");
    return head.next;
}
