#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/token.h"

void test_token();

int main() {
    test_token();
}

void test_token() {
    FILE* fp;
    fp = fopen("tests/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    assert(token->kind == TK_IDT);
    assert(token->next->kind == TK__EQ);
    assert(token->next->next->kind == TK_NUM);
}
