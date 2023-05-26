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

int check_kind(const Token* token, TokenKind* kinds) {
    int t_len = token_len(token);
    int k_len = ((int)sizeof(kinds) / (int)sizeof(TokenKind));
    if (t_len < k_len) return 0;
    const Token* cur = token;
    for (int i = 0; i < k_len; i++) {
        if (cur->kind != kinds[i]) {
            return 0;
        }
        cur = cur->next;
    }
    return 1;
}

Instr* _instrs(Token* token) {
    if (token == NULL) return NULL;

    Instr head;
    head.next = NULL;
    Instr* cur = &head;

    while (token->kind != TK_EOF) {
        // Label Instruction
        if (check_kind(token, (TokenKind[]){TK_IDT, TK_COL, TK_SEP})) {
            Node** nodes = calloc(1, sizeof(Node**));
            nodes[0] = new_node(ND_STR, (InnerValue){.string = token->string});
            token = token_skip(token, 3);
            cur = new_instr(cur, IN_LABEL, nodes);
            continue;
        }

        // Assign Instruction
        if (check_kind(token, (TokenKind[]){TK_IDT, TK__EQ})) {
            Node** nodes = calloc(1, sizeof(Node**));
            Token* token_ = token_skip(token, 2);
            nodes[0] = node_expr(token_);

            token_ = get_current_token();
            if (token_->kind == TK_SEP) {
                cur = new_instr(cur, IN_ASSIGN, nodes);
                token = token_skip(token_, 1);
                continue;
            }
        }

        // Goto Instruction
        if (check_kind(token, (TokenKind[]){TK__GO, TK_IDT})) {
            Token* token_ = (Token*)token_skip(token, 2);

            if (token_->kind == TK_SEP || token_->kind == TK_EOF) {
                Node** nodes = calloc(1, sizeof(Node**));
                nodes[0] = new_node(ND_STR, (InnerValue){.string = token->next->string});
                token = token_skip(token, 3);
                cur = new_instr(cur, IN_GOTO, nodes);
                continue;
            }
        }

        fprintf(stderr, "failed to compile (%s)\n", token->string);
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
