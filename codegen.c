#include "kzcc.h"

void generate_assembly_code(Node *node) {
    if (node->type == ND_NUM) {
        printf("\tpush %d\n", node->value);
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

    printf("\tpush rax\n");
}

