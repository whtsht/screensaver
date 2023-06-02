#ifndef EVAL_H
#define EVAL_H

#include "./instructions.h"
#include "./stack.h"
#include "./var.h"

typedef struct {
    // Program counter
    int pc;
    // Variables
    VarStack* vars;
    // Return Points
    Stack* return_points;
} Env;

Env* env_new();
int evaluate_node(Env* env, Node* node);
void evaluate(Env* env, InstrList instr_list);
void evaluate_debug(Env* env, InstrList instr_list);

#endif
