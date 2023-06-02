#ifndef VAR_H
#define VAR_H

#include "./hashmap.h"

typedef struct {
    char *name;
    int value;
} Var;

typedef struct VarMap VarMap;

struct VarMap {
    struct hashmap *map;
    VarMap *prev;
    VarMap *next;
};

typedef struct {
    VarMap *head;
    VarMap *tail;
} VarStack;

VarStack *new_var_stack();
void push_scope(VarStack *var_stack);
void pop_scope(VarStack *var_stack);
void set_var(VarStack *var_stack, char *name, int value);
int find_var(VarStack *var_stack, char *name);

#endif
