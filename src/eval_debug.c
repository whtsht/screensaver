#include "../inc/eval.h"

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
