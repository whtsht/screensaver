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

int check_kind(const Token* token, TokenKind* kinds, int length) {
    int t_len = token_len(token);
    int k_len = length;
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

void assert_and_skip_eol(Token** token) {
    if ((*token)->kind == TK_SEP || (*token)->kind == TK_EOF) {
        *token = token_skip(*token, 1);
    } else {
        fprintf(stderr, "expect newline or eof\n");
        exit(0);
    }
}

void assert_and_skip_token(Token** token, TokenKind kind) {
    if ((*token)->kind == kind) {
        *token = token_skip(*token, 1);
    } else {
        fprintf(stderr, "expect `");
        switch (kind) {
            case TK_NUM:
                fprintf(stderr, "number");
                break;
            case TK_RPA:
                fprintf(stderr, ")");
                break;
            case TK_LPA:
                fprintf(stderr, "(");
                break;
            default:
                fprintf(stderr, "TokenKind: %d", kind);
        }
        fprintf(stderr, "`");
    }
}

Instr* _instrs(Token* token) {
    if (token == NULL) return NULL;

    Instr head;
    head.next = NULL;
    Instr* cur = &head;

    while (token->kind != TK_EOF) {
        // Label Instruction
        if (check_kind(token, (TokenKind[]){TK_IDT, TK_COL}, 2)) {
            Node** nodes = calloc(1, sizeof(Node**));
            nodes[0] = new_node(ND_STR, (InnerValue){.string = token->string});
            token = token_skip(token, 2);

            assert_and_skip_eol(&token);
            cur = new_instr(cur, IN_LABEL, nodes);
            continue;
        }

        // Assign Instruction
        if (check_kind(token, (TokenKind[]){TK_IDT, TK__EQ}, 2)) {
            Node** nodes = calloc(2, sizeof(Node**));
            Token* token_ = token_skip(token, 2);
            nodes[0] = new_node(ND_STR, (InnerValue){.string = token->string});
            nodes[1] = node_expr(token_);

            token_ = get_current_token();

            assert_and_skip_eol(&token_);
            cur = new_instr(cur, IN_ASSIGN, nodes);
            token = token_;
            continue;
        }

        // Goto Instruction
        if (check_kind(token, (TokenKind[]){TK__GO, TK_IDT}, 2)) {
            Token* token_ = (Token*)token_skip(token, 2);

            if (token_->kind == TK_SEP || token_->kind == TK_EOF) {
                Node** nodes = calloc(1, sizeof(Node**));
                nodes[0] = new_node(ND_STR, (InnerValue){.string = token->next->string});
                token = token_skip(token, 3);
                cur = new_instr(cur, IN_GOTO, nodes);
                continue;
            }
        }

        // Call Instruction
        if (check_kind(token, (TokenKind[]){TK_IDT, TK_LPA}, 2)) {
            // number of arguments max 10
            Node** nodes = calloc(11, sizeof(Node**));
            nodes[0] = new_node(ND_STR, (InnerValue){.string = token->string});
            token = token_skip(token, 2);

            nodes[1] = node_expr(token);
            token = get_current_token();

            assert_and_skip_token(&token, TK_RPA);

            assert_and_skip_eol(&token);
            cur = new_instr(cur, IN_CALL, nodes);
            continue;
        }

        // Empty Line
        assert_and_skip_token(&token, TK_SEP);
        continue;

        fprintf(stderr, "failed to compile (%s)\n", token->string);
        exit(0);
    }

    return head.next;
}

int instr_len(const Instr* instrs) {
    if (instrs == NULL) return 0;
    int count = 1;
    const Instr* cur = instrs;
    while ((cur = cur->next)) {
        count += 1;
    }
    return count;
}

Instr** resolve_label(Instr* instrs) {
    if (instrs == NULL) return NULL;

    int len = instr_len(instrs);
    Instr** instr_array = calloc(len, sizeof(Instr**));
    Instr* cur = instrs;
    int idx = 0;
    do {
        instr_array[idx++] = cur;
    } while ((cur = cur->next));
    return instr_array;
}

InstrList gen_instrs(Token* token) {
    Instr* instrs = _instrs(token);
    int lenght = instr_len(instrs);
    Instr** list = resolve_label(instrs);
    return (InstrList){list, lenght};
}
