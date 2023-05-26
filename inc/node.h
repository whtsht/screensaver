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

struct BinaryOp {
    Node *lhs;
    Node *rhs;
};

struct UnaryOp {
    Node *node;
};

struct Node {
    NodeKind kind;
    union {
        BinaryOp binary;
        UnaryOp unary;
        int value;
    } inner;
};

Node *node_number(Token *token);

#endif
