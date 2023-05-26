#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/token.h"

int main() {
    FILE* fp;
    fp = fopen("tests/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    assert(token->kind == TK_IDT);
}
