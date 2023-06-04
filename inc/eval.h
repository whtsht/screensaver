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
int find_label(InstrList instr_list, char* label_name);
int find_func(InstrList instr_list, char* func_name);
int find_end(InstrList instr_list, int pc);
void enter_function(Env* env, char* name, Instr* instr, InstrList instr_list);
void exit_function(Env* env);
void debug_print(Env* env, Instr* instr);
int is_builtin(char* func_name);

#endif
