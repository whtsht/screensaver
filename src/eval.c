#include "../inc/eval.h"

Env* env_new() {
    Env* env = calloc(1, sizeof(Env));
    env->pc = 0;
    env->vars = new_var_stack();
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
        default:
            fprintf(stderr, "failed to evaluate node\n");
            exit(0);
    }
}

void evaluate(Env* env, InstrList instr_list) {
    fprintf(stderr, "TODO\n");
    exit(1);
}
