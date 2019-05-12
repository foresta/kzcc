#include "kzcc.h"

void generate_lvalue(Node *node) {
    if (node->type != ND_IDENT)
        error("lvalue is not variables. ");

    // varables is a to z 
    int offset = ('z' - node->name + 1) * 8;
    // load base pointer to rax
    printf("\tmov rax, rbp\n");
    // calcurate variable address. each variable (a to z)
    printf("\tsub rax, %d\n", offset);
    // add variable pointer addres to stack
    printf("\tpush rax\n");
}

void generate_assembly_code(Node *node) {
    if (node->type == ND_NUM) {
        printf("\tpush %d\n", node->value);
        return;
    }

    if (node->type == ND_IDENT) {
        // load variables
        
        // stack lvalue address
        generate_lvalue(node);
        printf("\tpop rax\n");
        // load lvalue from address
        printf("\tmov rax, [rax]\n");
        printf("\tpush rax\n");
    }

    if (node->type == '=') {
        // stack lvalue address
        generate_lvalue(node->lhs);

        // stack rvalue result
        generate_assembly_code(node->rhs);

        // pop rvalue
        printf("\tpop rdi\n");

        // pop lvalue
        printf("\tpop rax\n");

        // store rvalue to lvalue address
        printf("\tmov [rax], rdi");

        // stack rvalue
        // because a = 1 is return 1.
        printf("\tpush rdi\n");
        return;
    }

    generate_assembly_code(node->lhs);
    generate_assembly_code(node->rhs);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");

    switch (node->type) {
        case '+':
            printf("\tadd rax, rdi\n");
            break;
        case '-':
            printf("\tsub rax, rdi\n");
            break;
        case '*':
            printf("\tmul rdi\n");
            break;
        case '/':
            printf("\tmov rdx, 0\n");
            printf("\tdiv rdi\n");
            break;
        case TK_EQ:
            printf("\tcmp rax, rdi\n");
            printf("\tsete al\n");
            printf("\tmovzb rax, al\n");
            break;
        case TK_NE:
            printf("\tcmp rax, rdi\n");
            printf("\tsetne al\n");
            printf("\tmovzb rax, al\n");
            break;
        case '<':
        case '>':   // Always < because swap lhs and rhs if operator is >
            printf("\tcmp rax, rdi\n");
            printf("\tsetl al\n");
            printf("\tmovzb rax, al\n");
            break;
        case TK_LE:
        case TK_GE: // Always <= because swap lhs and rhs if operator is >=
            printf("\tcmp rax, rdi\n");
            printf("\tsetle al\n");
            printf("\tmovzb rax, al\n");
            break;

    }

    // stack result
    printf("\tpush rax\n");
}

