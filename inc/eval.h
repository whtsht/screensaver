#ifndef EVAL_H
#define EVAL_H

#include "./instructions.h"
#include "./var.h"

typedef struct {
    // Program counter
    int pc;
    // Variables
    VarStack* vars;
} Env;

Env* env_new();
int evaluate_node(Env* env, Node* node);
void evaluate(Env* env, InstrList instr_list);
void evaluate_debug(Env* env, InstrList instr_list);

#endif
