#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

//
// Vector
//

typedef struct {
    void **data;
    int capacity;
    int length;
} Vector;

Vector *new_vector() {
    int initial_capacity = 16;
    Vector *vec = malloc(sizeof(Vector));
    vec->data = malloc(sizeof(void *) * initial_capacity);
    vec->capacity = initial_capacity;
    vec->length = 0;
    return vec;
}

void vec_push(Vector *vec, void *element) {
    if (vec->capacity == vec->length) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }
    vec->data[vec->length++] = element;
}

//
// Token
//

// Value represented Token Type
enum {
    TK_NUM = 256,   // Integer Token
    TK_EQ,          // == Equal
    TK_NE,          // != Not Equal
    TK_LE,          // <= Less than or Equal
    TK_GE,          // >= Greater than or Equal
    TK_EOF,         // End of File Token
};

static struct {
    char *name;
    int type;
} symbols[] = {
    { "==", TK_EQ }, { "!=", TK_NE }, { "<=", TK_LE }, { ">=", TK_GE }, 
    { NULL, 0 },
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
Vector *tokens;
int pos = 0;

Token *get_token(int index) {
    return (Token *)tokens->data[index];
}

Token *new_token() {
    return calloc(1, sizeof(Token));
}

Node *equality();
Node *relational();
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
    if (get_token(pos)->type != type)
        return 0;

    pos++;
    return 1;
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume(TK_EQ))
            node = new_node(TK_EQ, node, relational());
        else if (consume(TK_NE))
            node = new_node(TK_NE, node, relational());
        else
            return node;
    }
}

Node *relational() {
    Node *node = add(); 

    for (;;) {
        if (consume('<'))
            node = new_node('<', node, add());
        else if (consume('>'))
            node = new_node('<', add(), node); // 左右のnodeを入れ替えて、'<' のみでパースするようにする
        if (consume(TK_LE))
            node = new_node(TK_LE, node, add());
        else if (consume(TK_GE))
            node = new_node(TK_GE, add(), node); // swap lhs and rhs
        else 
            return node;
    }
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
            error("There is no ) corresponding to (: %s", get_token(pos)->input);

        return node;
    }

    if (get_token(pos)->type == TK_NUM)
        return new_number_node(get_token(pos++)->value);

    error("invalid token. no number and no '(': %s", get_token(pos)->input);
}

void tokenize(char *p) {
    while (*p) {
        // skip space
        if (isspace(*p)) {
            p++;
            continue;
        }

        // Multi character symbol
        int multi_character_found = 0;
        for (int i = 0; symbols[i].name; i++) {
            char *name = symbols[i].name;
            int name_length = strlen(name);
            int match = !strncmp(p, name, name_length); // 文字列比較 (一致したら0が返ってくるため'!'が必要)
            if (!match) continue;

            Token *token = new_token();
            token->type = symbols[i].type;
            token->input = name;
            vec_push(tokens, token);
            p += name_length;

            multi_character_found = 1;
        }
        if (multi_character_found) continue;


        // Single charactor symbol
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>') {
            Token *token = new_token(); 
            token->type = *p;
            token->input = p;
            vec_push(tokens, token);
            p++;
            
            continue;
        }

        if (isdigit(*p)) {
            Token *token = new_token();
            token->type = TK_NUM;
            token->input = p;
            token->value = strtol(p, &p, 10);
            vec_push(tokens, token);
            continue;
        }

        error("Can't tokenize: %s", p);
        exit(1);
    }

    Token *token = new_token(); 
    token->type = TK_EOF;
    token->input = p;
    vec_push(tokens, token);
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

//
// Test
//

void expect(int line, int expected, int actual) {
    if (expected == actual)
        return;

    fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
    exit(1);
}

void runtest() {
    Vector *vec = new_vector();
    expect(__LINE__, 0, vec->length);

    for (int i = 0; i < 100; i++)
        vec_push(vec, (void *)(intptr_t)i);

    expect(__LINE__, 100, vec->length);
    expect(__LINE__, 0, (long)vec->data[0]);
    expect(__LINE__, 50, (long)vec->data[50]);
    expect(__LINE__, 99, (long)vec->data[99]);

    printf("OK\n");
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
    gen(node);

    // print last parts of assembler
    printf("\tpop rax\n");
    printf("\tret\n");
    
    return 0;
}


