#include "../inc/eval.h"

#include <stdio.h>
#include <string.h>

#include "../inc/stack.h"

Env* env_new() {
    Env* env = calloc(1, sizeof(Env));
    env->pc = 0;
    env->vars = new_var_stack();
    env->return_points = stack_new();
    return env;
}

int evaluate_node(Env* env, Node* node) {
    switch (node->kind) {
        case ND_NUM:
            return node->inner.value;
        case ND_VAR:
            return find_var(env->vars, node->inner.string);
        case ND_ADD: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs + rhs;
        }
        case ND_SUB: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs - rhs;
        }
        case ND_MUL: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs * rhs;
        }
        case ND_DIV: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs / rhs;
        }
        case ND_MOD: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs % rhs;
        }
        case ND_EEQ: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs == rhs;
        }
        case ND_NEQ: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs != rhs;
        }
        case ND__GT: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs > rhs;
        }
        case ND__GE: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs >= rhs;
        }
        case ND__LT: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs < rhs;
        }
        case ND__LE: {
            int lhs = evaluate_node(env, node->inner.binary.lhs);
            int rhs = evaluate_node(env, node->inner.binary.rhs);
            return lhs <= rhs;
        }
        default:
            fprintf(stderr, "failed to evaluate node\n");
            exit(0);
    }
}

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

void enter_function(Env* env, char* name, Instr* instr, InstrList instr_list) {
    int* point = calloc(1, sizeof(int));
    *point = env->pc + 1;
    stack_push(env->return_points, point);
    push_scope(env->vars);
    int func = find_func(instr_list, name);
    Instr* func_instr = instr_list.list[func];
    env->pc = func;

    int num_of_args = instr->nodes[1]->inner.value;

    for (int i = 2; i < num_of_args + 2; i++) {
        int value = evaluate_node(env, instr->nodes[i]);
        Node* arg = func_instr->nodes[i];
        if (arg->kind == ND_VAR) {
            set_var(env->vars, arg->inner.string, value);
        } else {
            fprintf(stderr, "argument must be a variable name (%s)\n", func_instr->nodes[0]->inner.string);
            exit(1);
        }
    }
}

void exit_function(Env* env) {
    pop_scope(env->vars);
}

void evaluate(Env* env, InstrList instr_list) {
    fprintf(stderr, "TODO\n");
    exit(1);
}
