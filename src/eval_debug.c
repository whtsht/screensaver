#include <stdio.h>
#include <string.h>

#include "../inc/eval.h"

void evaluate_debug(Env* env, InstrList instr_list) {
    while (env->pc < instr_list.length) {
        Instr* cur_instr = instr_list.list[env->pc];
        switch (instr_list.list[env->pc]->kind) {
            case IN_CALL: {
                char* name = cur_instr->nodes[0]->inner.string;
                int num_of_args = cur_instr->nodes[1]->inner.value;

                // debug print
                printf("call %s(", name);
                for (int i = 2; i < num_of_args + 2; i++) {
                    int value = evaluate_node(env, cur_instr->nodes[i]);
                    char str[20];
                    snprintf(str, 20, "%d", value);
                    if (i != num_of_args + 1) {
                        printf("%s ,", str);
                    } else {
                        printf("%s", str);
                    }
                }
                printf(")\n");
                // debug print end

                if (!is_builtin(name)) {
                    enter_function(env, name, cur_instr, instr_list);
                } else {
                    if (!strcmp(name, "debugPrint")) {
                        debug_print(env, cur_instr);
                    }
                    env->pc += 1;
                }
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
                env->pc = new_pc;
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

            case IN_FNDEF: {
                int* point = stack_top(env->return_points);
                if (point != NULL) {
                    printf("enter function %s\n", cur_instr->nodes[0]->inner.string);
                    env->pc += 1;
                } else {
                    int new_pc = find_end(instr_list, env->pc);
                    env->pc = new_pc;
                }
                break;
            }

            case IN_END: {
                int* point = stack_pop(env->return_points);
                if (point != NULL) {
                    printf("exit function\n");
                    exit_function(env);
                    env->pc = *point;
                    free(point);
                } else {
                    env->pc += 1;
                }
                break;
            }

            default:
                break;
        }
    }
}
