#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//
// Token
//

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


//
// AST
//

// Value represented Node Type
enum {
    ND_NUM = 256,   // Type of Integer Node
};

// Type of AST Node
typedef struct Node {
    int type; // operator or ND_NUM
    struct Node *lhs;   // left
    struct Node *rhs;   // right
    int value;          // used if type == ND_NUM
} Node;

// Tokenize result array
//   Suppose that the number of tokens is 100 or less
Token tokens[100];
int pos = 0;


Node *add();
Node *mul();
Node *unary();
Node *term();

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}


Node *new_node(int type, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_number_node(int value) {
    Node *node = malloc(sizeof(Node));
    node->type = ND_NUM;
    node->value = value;
    return node;
}

int consume(int type) {
    if (tokens[pos].type != type)
        return 0;

    pos++;
    return 1;
}


Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume('+'))
            node = new_node('+', node, mul());
        else if (consume('-'))
            node = new_node('-', node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node* node = unary();

    for (;;) {
        if (consume('*'))
            node = new_node('*', node, unary());
        else if (consume('/'))
            node = new_node('/', node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume('+'))
        return term();

    if (consume('-'))
        return new_node('-', new_number_node(0), term());

    return term();
}


Node *term() {
    if (consume('(')) {
        Node * node = add();
        if (!consume(')'))
            error("There is no ) corresponding to (: %s", tokens[pos].input);

        return node;
    }

    if (tokens[pos].type == TK_NUM)
        return new_number_node(tokens[pos++].value);

    error("invalid token. no number and no '(': %s", tokens[pos].input);
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
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
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

        error("Can't tokenize: %s", p);
        exit(1);
    }

    tokens[i].type = TK_EOF;
    tokens[i].input = p;
}


void gen(Node* node) {
    if (node->type == ND_NUM) {
        printf("\tpush %d\n", node->value);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

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
    }

    printf("\tpush rax\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数がただしくありません\n");
        return 1;
    }

    // Tokenize
    tokenize(argv[1]);

    // Parse and generate AST
    Node *node = add();

    // Print first parts of assembler.
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");


    // generate assembly code from AST
    gen(node);

    // print last parts of assembler
    printf("\tpop rax\n");
    printf("\tret\n");
    
    return 0;
}
