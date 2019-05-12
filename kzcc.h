#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdnoreturn.h>

#include "container.h"
#include "tokenizer.h"

// AST

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


//
// Variables
//

// unitily.c
noreturn void error(char *fmt, ...) __attribute__((format(printf,1,2)));

// test_utility.c
void expect(int line, int expected, int actual);

// codegen.c
void generate_assembly_code(Node* node);


// parser.c
Node *new_node(int type, Node *lhs, Node *rhs);
Node *new_number_node(int value);
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

