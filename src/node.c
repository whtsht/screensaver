#include "../inc/node.h"

#include <stdlib.h>
#include <string.h>

static Token *current_token = NULL;
Token *get_current_token() {
    return current_token;
}

Node *node_binary(Token *token, Node *(*func)(Token *), TokenKind token_kind, NodeKind node_kind) {
    if (token_len(token) < 3) return NULL;
    Token *lhs_ = token;
    Token *op = lhs_->next;
    Token *rhs_ = op->next;

    if (op->kind != token_kind) return NULL;

    Node *lhs = func(lhs_);
    if (lhs == NULL) return NULL;
    Node *rhs = node_expr(rhs_);
    if (rhs == NULL) return NULL;

    Node *node = new_node(node_kind, (InnerValue){.binary = {lhs, rhs}});
    current_token = rhs_->next;
    return node;
}

// 0: (), number, variable
Node *node_expr0(Token *token);

// 1: *, /, %
Node *node_expr1(Token *token);
Node *node_expr1_binary(Token *token, TokenKind token_kind, NodeKind node_kind) {
    return node_binary(token, node_expr0, token_kind, node_kind);
}

// 2: +, -
Node *node_expr2(Token *token);
Node *node_expr2_binary(Token *token, TokenKind token_kind, NodeKind node_kind) {
    return node_binary(token, node_expr1, token_kind, node_kind);
}

// 3: <, <=, >, >=
Node *node_expr3(Token *token);
Node *node_expr3_binary(Token *token, TokenKind token_kind, NodeKind node_kind) {
    return node_binary(token, node_expr2, token_kind, node_kind);
}

// 4: ==, !=
Node *node_expr4(Token *token);
Node *node_expr4_binary(Token *token, TokenKind token_kind, NodeKind node_kind) {
    return node_binary(token, node_expr3, token_kind, node_kind);
}

Node *new_node(NodeKind kind, InnerValue inner) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->inner = inner;
    return node;
}

Node *node_number(Token *token) {
    if (token->kind == TK_NUM) {
        Node *node = new_node(ND_NUM, (InnerValue){.value = atoi(token->string)});
        current_token = token->next;
        return node;
    }
    return NULL;
}

Node *node_var(Token *token) {
    if (token->kind == TK_IDT) {
        Node *node = new_node(ND_VAR, (InnerValue){.string = token->string});
        current_token = token->next;
        return node;
    }
    return NULL;
}

Node *node_add(Token *token) {
    return node_expr2_binary(token, TK_ADD, ND_ADD);
}

Node *node_sub(Token *token) {
    return node_expr2_binary(token, TK_SUB, ND_SUB);
}

Node *node_eeq(Token *token) {
    return node_expr4_binary(token, TK_EEQ, ND_EEQ);
}

Node *node_neq(Token *token) {
    return node_expr4_binary(token, TK_NEQ, ND_NEQ);
}

Node *node_expr0(Token *token) {
    Node *node = NULL;
    if ((node = node_number(token))) {
        return node;
    }
    if ((node = node_var(token))) {
        return node;
    }
    return NULL;
}

Node *node_expr1(Token *token) {
    return node_expr0(token);
}

Node *node_expr2(Token *token) {
    Node *node = NULL;
    if ((node = node_add(token))) {
        return node;
    }
    if ((node = node_sub(token))) {
        return node;
    }
    return node_expr1(token);
}

Node *node_expr3(Token *token) {
    return node_expr2(token);
}

Node *node_expr4(Token *token) {
    Node *node = NULL;
    if ((node = node_eeq(token))) {
        return node;
    }
    if ((node = node_neq(token))) {
        return node;
    }
    return node_expr3(token);
}

Node *node_expr(Token *token) {
    return node_expr4(token);
}
