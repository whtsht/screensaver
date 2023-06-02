#include "../inc/var.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int var_compare(const void *a, const void *b, void *_) {
    const Var *va = a;
    const Var *vb = b;
    return strcmp(va->name, vb->name);
}

uint64_t var_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const Var *var = item;
    return hashmap_sip(var->name, strlen(var->name), seed0, seed1);
}

VarStack *new_var_stack() {
    VarMap *map = calloc(1, sizeof(VarMap));
    map->map = hashmap_new(sizeof(Var), 0, 0, 0, var_hash, var_compare, NULL, NULL);
    map->next = NULL;
    map->prev = NULL;

    VarStack *var_stack = calloc(1, sizeof(VarStack));
    var_stack->head = map;
    var_stack->tail = map;

    return var_stack;
}

void push_scope(VarStack *var_stack) {
    VarMap *map = calloc(1, sizeof(VarMap));
    map->map = hashmap_new(sizeof(Var), 0, 0, 0, var_hash, var_compare, NULL, NULL);

    map->next = NULL;
    map->prev = var_stack->tail;

    var_stack->tail->next = map;
    var_stack->tail = map;
}

void pop_scope(VarStack *var_stack) {
    VarMap *tail = var_stack->tail;
    VarMap *new_tail = var_stack->tail->prev;

    new_tail->next = NULL;
    var_stack->tail = new_tail;

    hashmap_free(tail->map);
    free(tail);
}

void set_var(VarStack *var_stack, char *name, int value) {
    hashmap_set(var_stack->tail->map, &(Var){.name = name, .value = value});
}

int find_var(VarStack *var_stack, char *name) {
    VarMap *current = var_stack->tail;

    do {
        const Var *var = hashmap_get(current->map, &(Var){.name = name});
        if (var != NULL) {
            return var->value;
        }
    } while ((current = current->prev));

    fprintf(stderr, "cannot find var: (%s)\n", name);
    exit(1);
}
