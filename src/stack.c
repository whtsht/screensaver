#include "../inc/stack.h"

#include <stdio.h>
#include <stdlib.h>

Stack* stack_new() {
    Stack* stack = calloc(1, sizeof(Stack));
    stack->size = 0;
    stack->tail = NULL;
    return stack;
}

void stack_push(Stack* stack, void* value) {
    Element* tail = calloc(1, sizeof(Element));
    tail->value = value;
    if (stack->tail != NULL) {
        stack->tail->next = tail;
        tail->prev = stack->tail;
        stack->tail = tail;
    } else {
        stack->tail = tail;
    }
    stack->size += 1;
}

void* stack_top(Stack* stack) {
    return stack->tail;
}

void* stack_pop(Stack* stack) {
    if (stack->size == 0) {
        return NULL;
    }
    Element* tail = stack->tail;
    stack->tail = stack->tail->prev;
    if (stack->tail != NULL) {
        stack->tail->next = NULL;
    }

    stack->size -= 1;
    return tail->value;
}
