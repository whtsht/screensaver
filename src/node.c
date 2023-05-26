#include "../inc/node.h"

#include <stdlib.h>

Node *new_node() {
    return calloc(1, sizeof(Node));
}

Node *node_number(Token *token) {
    if (token->kind == TK_NUM) {
        Node *node = new_node();
        node->inner.value = atoi(token->string);
        return node;
    }
    return NULL;
}
