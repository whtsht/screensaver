#include <string.h>

#include "../inc/eval.h"

int find_label(InstrList instr_list, char* label_name) {
    for (int i = 0; i < instr_list.length; i++) {
        if (instr_list.list[i]->kind == IN_LABEL) {
            if (!strcmp(instr_list.list[i]->nodes[0]->inner.string, label_name)) {
                return i;
            }
        }
    }
    return -1;
}

void evaluate_debug(Env* env, InstrList instr_list) {
    while (env->pc < instr_list.length) {
        Instr* cur_instr = instr_list.list[env->pc];
        switch (instr_list.list[env->pc]->kind) {
            case IN_CALL: {
                printf("call %s\n", cur_instr->nodes[0]->inner.string);
                env->pc += 1;
                break;
            }
            case IN_LABEL: {
                env->pc += 1;
                break;
            }
            case IN_ASSIGN: {
                char* name = cur_instr->nodes[0]->inner.string;
                printf("%s = %d\n", name, evaluate_node(env, cur_instr->nodes[1]));
                int value = evaluate_node(env, cur_instr->nodes[1]);
                set_var(env->vars, name, value);
                env->pc += 1;
                break;
            }
            case IN_COND_GOTO: {
                int value = evaluate_node(env, cur_instr->nodes[1]);
                if (value == 0) {
                    env->pc += 1;
                    break;
                }
                char* label = cur_instr->nodes[0]->inner.string;
                int new_pc = find_label(instr_list, label);
                if (new_pc != -1) {
                    env->pc = new_pc;
                    break;
                } else {
                    fprintf(stderr, "not found label %s\n", label);
                    exit(1);
                }
                break;
            }
            case IN_GOTO: {
                char* label = cur_instr->nodes[0]->inner.string;
                int new_pc = find_label(instr_list, label);
                if (new_pc != -1) {
                    env->pc = new_pc;
                    break;
                } else {
                    fprintf(stderr, "not found label %s\n", label);
                    exit(1);
                }
            }
            default:
                break;
        }
    }
}
