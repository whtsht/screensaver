#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/instructions.h"
#include "../inc/lang.h"
#include "../inc/node.h"

int main() {
    FILE* fp;
    fp = fopen("./examples/triangle.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    Instr** instr = gen_instrs(token);
    printf("%s\n", instr[0]->nodes[0]->inner.string);
    printf("%s\n", instr[1]->nodes[0]->inner.string);
    printf("%s\n", instr[2]->nodes[0]->inner.string);
}
