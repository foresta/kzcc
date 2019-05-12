#include "kzcc.h"

//
// Test
//
void runtest() {
    test_vector();
    test_map();
}

//
// Debug
//
void ast_debug() {
    debug("=====[print ast]=====");
    for (int i = 0; code[i]; i++) {
        print_ast(code[i]);
    }
    debug("=====================");
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
    // Node* code[100] is generated
    program();

    // Print first parts of assembler.
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // Prologue
    // allocate memory for variable (a to z, 26)
    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, 208\n");

    for (int i = 0; code[i]; i++) {

        // generate assembly code from AST
        generate_assembly_code(code[i]);

        // Pop skack so that it doesn't overflow, 
        // because one value remain on the stack as a result of expression evaluation.
        printf("\tpop rax\n");
    }

    // Epilogue
    // print last parts of assembler
    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    
    return 0;
}

