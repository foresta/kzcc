#include "kzcc.h"

void print_mnemonic(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("\t");
    vprintf(fmt, ap);
    printf("\n");
}

void generate_lvalue(Node *node) {
    if (node->type != ND_IDENT)
        error("lvalue is not variables. ");

    // varables is a to z 
    int offset = ('z' - node->name + 1) * 8;
    // load base pointer to rax
    print_mnemonic("mov rax, rbp");
    // calcurate variable address. each variable (a to z)
    print_mnemonic("sub rax, %d", offset);
    // add variable pointer addres to stack
    print_mnemonic("push rax");
}

void generate_assembly_code(Node *node) {
    if (node->type == ND_NUM) {
        print_mnemonic("push %d", node->value);
        return;
    }

    if (node->type == ND_IDENT) {
        // load variables
        
        // stack lvalue address
        generate_lvalue(node);
        print_mnemonic("pop rax");
        // load lvalue from address
        print_mnemonic("mov rax, [rax]");
        print_mnemonic("push rax");
        return;
    }

    if (node->type == '=') {
        // stack lvalue address
        generate_lvalue(node->lhs);

        // stack rvalue result
        generate_assembly_code(node->rhs);

        // pop rvalue
        print_mnemonic("pop rdi");

        // pop lvalue
        print_mnemonic("pop rax");

        // store rvalue to lvalue address
        print_mnemonic("mov [rax], rdi");

        // stack rvalue
        // because a = 1 is return 1.
        print_mnemonic("push rdi");
        return;
    }

    generate_assembly_code(node->lhs);
    generate_assembly_code(node->rhs);

    print_mnemonic("pop rdi");
    print_mnemonic("pop rax");

    switch (node->type) {
        case '+':
            print_mnemonic("add rax, rdi");
            break;
        case '-':
            print_mnemonic("sub rax, rdi");
            break;
        case '*':
            print_mnemonic("mul rdi");
            break;
        case '/':
            print_mnemonic("mov rdx, 0");
            print_mnemonic("div rdi");
            break;
        case TK_EQ:
            print_mnemonic("cmp rax, rdi");
            print_mnemonic("sete al");
            print_mnemonic("movzb rax, al");
            break;
        case TK_NE:
            print_mnemonic("cmp rax, rdi");
            print_mnemonic("setne al");
            print_mnemonic("movzb rax, al");
            break;
        case '<':
        case '>':   // Always < because swap lhs and rhs if operator is >
            print_mnemonic("cmp rax, rdi");
            print_mnemonic("setl al");
            print_mnemonic("movzb rax, al");
            break;
        case TK_LE:
        case TK_GE: // Always <= because swap lhs and rhs if operator is >=
            print_mnemonic("cmp rax, rdi");
            print_mnemonic("setle al");
            print_mnemonic("movzb rax, al");
            break;

    }

    // stack result
    print_mnemonic("push rax");
}

