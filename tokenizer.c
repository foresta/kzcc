#include "kzcc.h"
#
// Tokenize result array
Vector *tokens;

// Tokenize Cursor
int pos = 0;

// Tokenize Symbol
const Symbol symbols[5] = {
    { "==", TK_EQ },
    { "!=", TK_NE },
    { "<=", TK_LE },
    { ">=", TK_GE },
    { NULL, 0 },
};

Token *get_token(int index) {
    return (Token *)tokens->data[index];
}

Token *new_token() {
    return calloc(1, sizeof(Token));
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
