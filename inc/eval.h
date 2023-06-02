#ifndef EVAL_H
#define EVAL_H

#include "./instructions.h"

typedef struct {
    // Program counter
    int pc;
} Env;

int evaluate_node(Env env, Node* node);
void evaluate(Env env, InstrList instr_list);
void evaluate_debug(Env env, InstrList instr_list);

#endif
