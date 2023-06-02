#include "../inc/eval.h"

int evaluate_node(Env env, Node* node) {
    switch (node->kind) {
        case ND_NUM:
            return node->inner.value;
        default:
            fprintf(stderr, "failed to evaluate node\n");
            exit(0);
    }
}

void evaluate(Env env, InstrList instr_list) {
    while (env.pc < instr_list.length) {
        switch (instr_list.list[env.pc]->kind) {
            case IN_CALL:
                printf("call %s\n", instr_list.list[env.pc]->nodes[0]->inner.string);
                env.pc += 1;
                break;
            default:
                break;
        }
    }
}
