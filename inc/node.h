#ifndef NODE_H
#define NODE_H

#include "token.h"

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_MOD,
    ND_STR,
    ND_VAR,
    ND_EEQ,
    ND_NEQ,
    ND__GT,
    ND__GE,
    ND__LT,
    ND__LE
} NodeKind;

typedef struct Node Node;
typedef struct BinaryOp BinaryOp;
typedef struct UnaryOp UnaryOp;
typedef union Inner InnerValue;

struct BinaryOp {
    Node *lhs;
    Node *rhs;
};

struct UnaryOp {
    Node *node;
};

union Inner {
    BinaryOp binary;
    UnaryOp unary;
    int value;
    char *string;
};

struct Node {
    NodeKind kind;
    InnerValue inner;
};

Node *node_number(Token *token);
Node *node_add(Token *token);
Node *node_expr(Token *token);
Node *new_node(NodeKind kind, InnerValue inner);
Token *get_current_token();

#endif
