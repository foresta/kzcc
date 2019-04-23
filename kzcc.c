#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Value represented Token Type
enum {
    TK_NUM = 256,   // Integer Token
    TK_EOF,         // End of File Token
};

// Type of Token
typedef struct {
    int type;    // Type of token
    int value;   // Token Value if type is TK_NUM 
    char *input; // Token Text for Error Msg
} Token;

// Tokenize result array
//   Suppose that the number of tokens is 100 or less
Token tokens[100];

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void tokenize(char *p) {
    int i = 0;
    while (*p) {
        // skip space
        if (isspace(*p)) {
            p++;
            continue;
        }

        // add and sub
        if (*p == '+' || *p == '-') {
            tokens[i].type = *p;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            tokens[i].type = TK_NUM;
            tokens[i].input = p;
            tokens[i].value = strtol(p, &p, 10);
            i++;
            continue;
        }

        error("Can't tokenize: $s", p);
        exit(1);
    }

    tokens[i].type = TK_EOF;
    tokens[i].input = p;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数がただしくありません\n");
        return 1;
    }

    // Tokenize
    tokenize(argv[1]);

    // print first parts of assembler.
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    
    if (tokens[0].type != TK_NUM)
        error("first term is not number");
    
    printf("\tmov rax, %d\n", tokens[0].value);

    int i = 1;
    while (tokens[i].type != TK_EOF) {
        if (tokens[i].type == '+') {
            i++;
            if (tokens[i].type != TK_NUM)
                error("Unexpected token: %s", tokens[i].input);

            printf("\tadd rax, %d\n", tokens[i].value);
            i++;
            continue;
        }

        if (tokens[i].type == '-') {
            i++;
            if (tokens[i].type != TK_NUM)
                error("Unexpected token: %s", tokens[i].input);

            printf("\tsub rax, %d\n", tokens[i].value);
            i++;
            continue;
        }

        error("Unexpected token: %s", tokens[i].input);
    }

    // print last parts of assembler
    printf("\tret\n");
    
    return 0;
}
