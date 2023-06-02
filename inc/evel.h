#ifndef EVAL_H
#define EVAL_H

#include "./instructions.h"

typedef struct {
    // Program counter
    int pc;
} Env;

void evalute(Env env, InstrList instr_list);
void evalute_debug(Env env, InstrList instr_list);

#endif
