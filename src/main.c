#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/lang.h"
#include "../inc/node.h"

int main() {
    FILE* fp;
    fp = fopen("./examples/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    Node* node = node_expr(token);
    printf("%d\n", node->kind);
}
