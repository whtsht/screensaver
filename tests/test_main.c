#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/eval.h"
#include "../inc/instructions.h"
#include "../inc/node.h"
#include "../inc/token.h"

int strcmp_(char* s1, char* s2) {
    return !strcmp(s1, s2);
}

void test_token();
void test_instruction();
void test_eval_node();

int main() {
    test_token();
    test_instruction();
    test_eval_node();
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

void test_call_instruction();
void test_simple_instruction();

void test_instruction() {
    test_call_instruction();
    test_simple_instruction();
}

void test_simple_instruction() {
    FILE* fp;
    fp = fopen("tests/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    InstrList instrs = gen_instrs(token);

    assert(instrs.list[0]->kind == IN_LABEL);
    assert(strcmp_(instrs.list[0]->nodes[0]->inner.string, "A"));
    assert(instrs.list[1]->kind == IN_ASSIGN);
    assert(strcmp_(instrs.list[1]->nodes[0]->inner.string, "val"));
    assert(instrs.list[1]->nodes[1]->inner.value == 10);
    assert(instrs.list[2]->kind == IN_GOTO);
    assert(strcmp_(instrs.list[2]->nodes[0]->inner.string, "A"));

    fclose(fp);
}

void test_call_instruction() {
    FILE* fp;
    fp = fopen("tests/call.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    assert(token->kind == TK_IDT);
    assert(token->next->kind == TK_LPA);
    assert(token->next->next->kind == TK_NUM);
    assert(token->next->next->next->kind == TK_RPA);

    InstrList instrs = gen_instrs(token);

    assert(instrs.list[0]->kind == IN_CALL);

    fclose(fp);
}

void test_eval_simple();
void test_eval_expr();

void test_eval_node() {
    test_eval_simple();
    test_eval_expr();
}

void test_eval_simple() {
    FILE* fp;
    fp = fopen("tests/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    InstrList instrs = gen_instrs(token);

    assert(instrs.list[1]->nodes[1]->inner.value == 10);
    Env* env = env_new();
    assert(evaluate_node(env, instrs.list[1]->nodes[1]) == 10);

    fclose(fp);
}

void test_expr(Node* node, int expect) {
    Env* env = env_new();
    int value = evaluate_node(env, node);
    assert(expect == value);
}

void test_eval_expr() {
    FILE* fp;
    fp = fopen("tests/expr.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    InstrList instrs = gen_instrs(token);

    test_expr(instrs.list[0]->nodes[1], 10);
    test_expr(instrs.list[1]->nodes[1], 10);
    test_expr(instrs.list[2]->nodes[1], 12);
    test_expr(instrs.list[3]->nodes[1], 2);
    test_expr(instrs.list[4]->nodes[1], -9);
    test_expr(instrs.list[5]->nodes[1], 1);
    test_expr(instrs.list[6]->nodes[1], 0);
    test_expr(instrs.list[7]->nodes[1], 1);
    test_expr(instrs.list[8]->nodes[1], 1);
    test_expr(instrs.list[9]->nodes[1], 0);
    test_expr(instrs.list[10]->nodes[1], 1);
    test_expr(instrs.list[11]->nodes[1], 1);
    test_expr(instrs.list[12]->nodes[1], 0);
    test_expr(instrs.list[13]->nodes[1], 1);
    test_expr(instrs.list[14]->nodes[1], 0);

    fclose(fp);
}
