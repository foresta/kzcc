
// Value represented Token Type
enum {
    TK_NUM = 256,   // Integer Token
    TK_IDENT,      // [a-z]
    TK_EQ,          // == Equal
    TK_NE,          // != Not Equal
    TK_LE,          // <= Less than or Equal
    TK_GE,          // >= Greater than or Equal
    TK_RETURN,      // return
    TK_EOF,         // End of File Token
};

// Type of Token
typedef struct {
    int type;    // Type of token
    int value;   // Token Value if type is TK_NUM 
    char *input; // Token Text for Error Msg
} Token;

// Symbol
typedef struct {
    char *name;
    int type;
} Symbol;

// Keyword
typedef struct {
    char *name;
    int type;
} Keyword;

Token *get_token(int index);
Token *new_token(void);

void tokenize(char *p);

// Tokenize result array
extern Vector *tokens;

// Tokenize Cursor
extern int pos;


