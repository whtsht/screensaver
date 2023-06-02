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
    fprintf(stderr, "label (%s) not found\n", label_name);
    exit(1);
}

int is_builtin(char* func_name) {
    return strcmp("penDown", func_name) == 0 ||
           strcmp("rotate", func_name) == 0 ||
           strcmp("forward", func_name) == 0 ||
           strcmp("penUp", func_name) == 0;
}

int find_func(InstrList instr_list, char* func_name) {
    for (int i = 0; i < instr_list.length; i++) {
        if (instr_list.list[i]->kind == IN_FNDEF) {
            if (!strcmp(instr_list.list[i]->nodes[0]->inner.string, func_name)) {
                return i;
            }
        }
    }
    fprintf(stderr, "function (%s) not found\n", func_name);
    exit(1);
}

int find_end(InstrList instr_list, int pc) {
    for (int i = pc; i < instr_list.length; i++) {
        if (instr_list.list[i]->kind == IN_END) {
            return i;
        }
    }
    fprintf(stderr, "this function is not closed\n");
    exit(1);
}

void evaluate_debug(Env* env, InstrList instr_list) {
    while (env->pc < instr_list.length) {
        Instr* cur_instr = instr_list.list[env->pc];
        switch (instr_list.list[env->pc]->kind) {
            case IN_CALL: {
                char* name = cur_instr->nodes[0]->inner.string;
                printf("call %s\n", name);
                if (!is_builtin(name)) {
                    int* point = calloc(1, sizeof(int));
                    *point = env->pc + 1;
                    stack_push(env->return_points, point);
                    env->pc = find_func(instr_list, name);
                } else {
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
                    env->pc = *point;
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
