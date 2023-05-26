#include "../inc/instructions.h"

#include <stdio.h>
#include <stdlib.h>

Instr* new_instr(Instr* current, InstrKind kind, Node** nodes) {
    Instr* instr = calloc(1, sizeof(Instr));

    instr->kind = kind;
    instr->next = NULL;
    instr->nodes = nodes;
    current->next = instr;
    return instr;
}

Instr* _instrs(Token* token) {
    if (token == NULL) return NULL;

    Instr head;
    head.next = NULL;
    Instr* cur = &head;

    while (token->kind != TK_EOF) {
        if (token_len(token) >= 2 &&
            token->kind == TK_IDT &&
            token->next->kind == TK_COL) {
            Node** nodes = calloc(1, sizeof(Node**));
            nodes[0] = new_node(ND_STR, (InnerValue){.string = token->string});
            token = token->next->next;
            cur = new_instr(cur, IN_LABEL, nodes);
        }

        if (token->kind == TK_SEP) {
            token = token->next;
            continue;
        }

        fprintf(stderr, "failed to compile\n");
        exit(0);
    }

    return head.next;
}

Instr* resolve_label(Instr* instrs) {
    return instrs;
}

Instr* gen_instrs(Token* token) {
    Instr* instrs = _instrs(token);
    return resolve_label(instrs);
}
