#include "../inc/node.h"

#include <stdlib.h>
#include <string.h>

Node *new_node(NodeKind kind, InnerValue inner) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->inner = inner;
    return node;
}

Node *node_number(Token *token) {
    if (token->kind == TK_NUM) {
        Node *node = new_node(ND_NUM, (InnerValue){.value = atoi(token->string)});
        return node;
    }
    return NULL;
}

Node *node_add(Token *token) {
    if (token_len(token) < 3) return NULL;
    Token *lhs_ = token;
    Token *op = lhs_->next;
    Token *rhs_ = op->next;

    if (strcmp(op->string, "+")) return NULL;

    Node *lhs = node_number(lhs_);
    if (lhs == NULL) return NULL;
    Node *rhs = node_expr(rhs_);
    if (rhs == NULL) return NULL;

    Node *node = new_node(ND_ADD, (InnerValue){.binary = {lhs, rhs}});
    return node;
}

Node *node_sub(Token *token) {
    if (token_len(token) < 3) return NULL;
    Token *lhs_ = token;
    Token *op = lhs_->next;
    Token *rhs_ = op->next;

    if (strcmp(op->string, "-")) return NULL;

    Node *lhs = node_number(lhs_);
    if (lhs == NULL) return NULL;
    Node *rhs = node_expr(rhs_);
    if (rhs == NULL) return NULL;

    Node *node = new_node(ND_SUB, (InnerValue){.binary = {lhs, rhs}});
    return node;
}

Node *node_expr(Token *token) {
    Node *node = NULL;
    if ((node = node_add(token))) {
        return node;
    }
    if ((node = node_sub(token))) {
        return node;
    }
    if ((node = node_number(token))) {
        return node;
    }
    return NULL;
}
