#ifndef NODE_H
#define NODE_H

#include "token.h"

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
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
};

struct Node {
    NodeKind kind;
    InnerValue inner;
};

Node *node_number(Token *token);
Node *node_add(Token *token);
Node *node_expr(Token *token);

#endif
