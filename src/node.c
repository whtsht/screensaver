#include "../inc/node.h"

#include <stdlib.h>
#include <string.h>

static Token *current_token = NULL;
Token *get_current_token() {
    return current_token;
}

// Left-to-Right Associativity
Node *node_binary(Token *token, Node *(*func)(Token *), TokenKind token_kind[], NodeKind node_kind[], int length) {
    Node *node = func(token);
    while (1) {
        Token *op = get_current_token();
        int flag = 0;
        for (int i = 0; i < length; i++) {
            if (op->kind == token_kind[i]) {
                token = op->next;
                node = new_node(node_kind[i], (InnerValue){.binary = {node, func(token)}});
                flag = 1;
                break;
            }
        }
        if (!flag) {
            return node;
        }
    }
}

// 0: (), number, variable
Node *node_expr0(Token *token);

// 1: *, /, %
Node *node_expr1(Token *token);

// 2: +, -
Node *node_expr2(Token *token);

// 3: <, <=, >, >=
Node *node_expr3(Token *token);

// 4: ==, !=
Node *node_expr4(Token *token);

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

Node *node_parens(Token *token) {
    if (token->kind == TK_LPA) {
        Node *node = node_expr(token->next);
        token = get_current_token();
        if (node != NULL && token->kind == TK_RPA) {
            current_token = token->next;
            return node;
        }
    }
    return NULL;
}

Node *node_expr0(Token *token) {
    Node *node = NULL;
    if ((node = node_number(token))) {
        return node;
    }
    if ((node = node_var(token))) {
        return node;
    }
    if ((node = node_parens(token))) {
        return node;
    }
    return NULL;
}

Node *node_expr1(Token *token) {
    return node_binary(
        token,
        node_expr0,
        (TokenKind[]){TK_MUL, TK_DIV, TK_MOD},
        (NodeKind[]){ND_MUL, ND_DIV, ND_MOD}, 3);
}

Node *node_expr2(Token *token) {
    return node_binary(
        token,
        node_expr1,
        (TokenKind[]){TK_ADD, TK_SUB},
        (NodeKind[]){ND_ADD, ND_SUB}, 2);
}

Node *node_expr3(Token *token) {
    return node_expr2(token);
}

Node *node_expr4(Token *token) {
    return node_binary(
        token,
        node_expr3,
        (TokenKind[]){TK_EEQ, TK_NEQ},
        (NodeKind[]){ND_EEQ, ND_NEQ},
        2);
}

Node *node_expr(Token *token) {
    return node_expr4(token);
}
