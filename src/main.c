#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/instructions.h"
#include "../inc/lang.h"
#include "../inc/node.h"

int main() {
    FILE* fp;
    fp = fopen("./examples/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    Instr* instr = gen_instrs(token);
    printf("%s\n", instr->nodes[0]->inner.string);
    printf("%s\n", instr->next->next->nodes[0]->inner.string);
    printf("%s\n", instr->next->next->next->nodes[0]->inner.string);
}
