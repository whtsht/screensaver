#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/eval.h"
#include "../inc/hashmap.h"
#include "../inc/instructions.h"
#include "../inc/lang.h"
#include "../inc/node.h"
#include "../inc/stack.h"
#include "../inc/var.h"

int main() {
    FILE *fp;
    fp = fopen("./examples/triangle.tur", "r");
    Stream *stream = to_stream(fp);

    Token *token = tokenize(stream);
    free_stream(stream);

    InstrList instr = gen_instrs(token);
    Env *env = env_new();
    evaluate_debug(env, instr);
}
