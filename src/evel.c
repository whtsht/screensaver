#include "../inc/evel.h"

void evalute_debug(Env env, InstrList instr_list) {
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
