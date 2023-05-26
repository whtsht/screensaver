#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "node.h"

typedef enum {
    // A:
    IN_LABEL,
    // goto A
    IN_GOTO,
    // cgoto A a == b
    IN_COND_GOTO,
    // forward(10)
    IN_FUNC,
    // val = 5
    IN_ASSIGN,
} InstrKind;

typedef struct Instr Instr;

struct Instr {
    InstrKind kind;
    Node** nodes;
    Instr* next;
};

Instr* gen_instrs(Token* token);

#endif
