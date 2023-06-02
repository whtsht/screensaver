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

char stream_peek_with_offset(const Stream* stream, int offset) {
    if (stream_iseof(stream)) {
        return EOF;
    }
    return stream->string[stream->current + offset];
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

char* RESERVED_WORD[] = {"goto", "cgoto"};
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
        if (stream_peek(stream) == ':') {
            cur = new_token(cur, TK_COL, ":");
            stream_next(stream);
            continue;
        }
        if (stream_peek(stream) == ',') {
            cur = new_token(cur, TK_COM, ",");
            stream_next(stream);
            continue;
        }

        if (stream_peek(stream) == '=') {
            if (stream_peek_with_offset(stream, 1) == '=') {
                cur = new_token(cur, TK_EEQ, "==");
                stream_next(stream);
                stream_next(stream);
            } else {
                cur = new_token(cur, TK__EQ, "=");
                stream_next(stream);
            }
            continue;
        }

        if (stream_peek(stream) == '<') {
            if (stream_peek_with_offset(stream, 1) == '=') {
                cur = new_token(cur, TK__LE, "<=");
                stream_next(stream);
                stream_next(stream);
            } else {
                cur = new_token(cur, TK__LT, "<");
                stream_next(stream);
            }
            continue;
        }

        if (stream_peek(stream) == '>') {
            if (stream_peek_with_offset(stream, 1) == '=') {
                cur = new_token(cur, TK__GE, ">=");
                stream_next(stream);
                stream_next(stream);
            } else {
                cur = new_token(cur, TK__GT, ">");
                stream_next(stream);
            }
            continue;
        }

        if (stream_peek(stream) == '!') {
            if (stream_peek_with_offset(stream, 1) == '=') {
                cur = new_token(cur, TK_NEQ, "!=");
                stream_next(stream);
                stream_next(stream);
                continue;
            }
        }

        char* digit = parse_digit(stream);
        if (digit != NULL) {
            cur = new_token(cur, TK_NUM, digit);
            continue;
        }

        char* ident = parse_ident(stream);

        if (ident != NULL) {
            if (!strcmp(ident, "goto")) {
                cur = new_token(cur, TK__GO, "goto");
                continue;
            } else if (!strcmp(ident, "cgoto")) {
                cur = new_token(cur, TK_CGO, "cgoto");
                continue;
            } else if (!strcmp(ident, "def")) {
                cur = new_token(cur, TK_DEF, "def");
                continue;
            } else if (!strcmp(ident, "end")) {
                cur = new_token(cur, TK_END, "end");
                continue;
            } else {
                cur = new_token(cur, TK_IDT, ident);
                continue;
            }
        }

        fprintf(stderr, "failed to tokenize: (%d)", stream_peek(stream));
        exit(0);
    }

    cur = new_token(cur, TK_EOF, "");
    return head.next;
}

int token_len(const Token* token) {
    if (token == NULL) return 0;

    int count = 0;
    const Token* cur = token;
    while ((cur = cur->next)) {
        count += 1;
    }
    return count;
}

Token* token_skip(Token* token, int n) {
    for (int i = 0; i < n; i++) {
        token = token->next;
    }
    return token;
}

void token_skip_mut(Token** token, int n) {
    Token* cur = *token;
    for (int i = 0; i < n; i++) {
        cur = cur->next;
    }
    *token = cur;
}
