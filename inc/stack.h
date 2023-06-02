#ifndef STACK_H
#define STACK_H

typedef struct Element Element;

struct Element {
    void* value;
    Element* next;
    Element* prev;
};

typedef struct {
    int size;
    Element* tail;
} Stack;

Stack* stack_new();
void stack_push(Stack* stack, void* value);
void* stack_pop(Stack* stack);
void* stack_top(Stack* stack);

#endif
