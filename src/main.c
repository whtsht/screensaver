#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/eval.h"
#include "../inc/instructions.h"
#include "../inc/lang.h"
#include "../inc/node.h"

int main() {
    FILE* fp;
    fp = fopen("./examples/triangle.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    free_stream(stream);

    InstrList instr = gen_instrs(token);
    Env env = {0};
    evaluate_debug(env, instr);
}
