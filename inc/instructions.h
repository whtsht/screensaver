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
    // call forward(10, 2)
    IN_CALL,
    // val = 5
    IN_ASSIGN,
    // def xxx()
    IN_FNDEF,
    // end
    IN_END
} InstrKind;

typedef struct Instr Instr;

struct Instr {
    InstrKind kind;
    Node** nodes;
    Instr* next;
};

typedef struct {
    Instr** list;
    int length;
} InstrList;

InstrList gen_instrs(Token* token);

#endif
