// AST

// Value represented Node Type
enum {
    ND_NUM = 256,   // Type of Integer Node
    ND_IDENT,       // Type of Identifier Node
};

// Type of AST Node
typedef struct Node {
    int type; // operator or ND_NUM
    struct Node *lhs;   // left
    struct Node *rhs;   // right
    int value;          // used if type == ND_NUM
    char name;          // used if type == ND_IDENT
} Node;

// parser.c
Node *new_node(int type, Node *lhs, Node *rhs);
Node *new_number_node(int value);

void program();
Node *assign();
Node *stmt();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

// program code
extern Node *code[100];

// Parse Cursor
extern int pos;


