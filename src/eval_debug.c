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

void evaluate_debug(Env env, InstrList instr_list) {
    while (env.pc < instr_list.length) {
        Instr* cur_instr = instr_list.list[env.pc];
        switch (instr_list.list[env.pc]->kind) {
            case IN_CALL: {
                int value = evaluate_node(env, cur_instr->nodes[1]);
                printf("call %s(%d)\n", instr_list.list[env.pc]->nodes[0]->inner.string, value);
                env.pc += 1;
                break;
            }
            case IN_LABEL: {
                env.pc += 1;
                break;
            }
            default:
                break;
        }
    }
}
