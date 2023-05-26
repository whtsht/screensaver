#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/lang.h"

int main() {
    FILE* fp;
    fp = fopen("./examples/simple.tur", "r");
    Stream* stream = to_stream(fp);

    Token* token = tokenize(stream);
    printf("%s\n", token->string);
    printf("%s\n", token->next->string);
    printf("%s\n", token->next->next->string);
    printf("%s\n", token->next->next->next->string);
}
