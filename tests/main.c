#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/instructions.h"
#include "../inc/token.h"

void test_token();
void test_instruction();

int main() {
    test_token();
    test_instruction();
}

void test_token() {
    FILE* fp;
    fp = fopen("tests/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    assert(token->kind == TK_IDT);
    assert(token->next->kind == TK_COL);
    assert(token->next->next->kind == TK_SEP);

    fclose(fp);
}

int strcmp_(char* s1, char* s2) {
    return !strcmp(s1, s2);
}

void test_instruction() {
    FILE* fp;
    fp = fopen("tests/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    Instr** instrs = gen_instrs(token);

    assert(instrs[0]->kind == IN_LABEL);
    assert(strcmp_(instrs[0]->nodes[0]->inner.string, "A"));
    assert(instrs[1]->kind == IN_ASSIGN);
    assert(strcmp_(instrs[1]->nodes[0]->inner.string, "val"));
    assert(instrs[1]->nodes[1]->inner.value == 10);
    assert(instrs[2]->kind == IN_GOTO);
    assert(strcmp_(instrs[2]->nodes[0]->inner.string, "A"));

    fclose(fp);
}
