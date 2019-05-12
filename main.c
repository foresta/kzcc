#include "kzcc.h"

//
// Test
//
void runtest() {
    test_vector();
}

//
// Main
//

int main(int argc, char **argv) {
    if (argc != 2)
        error("引数の個数がただしくありません");

    if (argc == 2 && !strcmp(argv[1], "-test")) {
        runtest();
        return 0;
    }

    tokens = new_vector();

    // Tokenize
    tokenize(argv[1]);

    // Parse and generate AST
    Node *node = equality();

    // Print first parts of assembler.
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");


    // generate assembly code from AST
    generate_assembly_code(node);

    // print last parts of assembler
    printf("\tpop rax\n");
    printf("\tret\n");
    
    return 0;
}

