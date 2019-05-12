#include "kzcc.h"

void print_ast_node(Node *node, char *prefix) {
    if (node->type == ND_NUM) {
        debug("%sNUM: %d", prefix, node->value);       
    } else if (node->type == ND_IDENT) {
        debug("%sVAR: %c", prefix, node->name);       
    } else if (node->type == ND_EQ) {
        debug("%sEQ: ==", prefix);       
    } else if (node->type == ND_NE) {
        debug("%sNE: !=", prefix);       
    } else if (node->type == ND_LE) {
        debug("%sLE: <=", prefix);       
    } else if (node->type == ND_GE) {
        debug("%sGE: >=", prefix);       
    } else if (node->type == ND_RETURN) {
        debug("%sRETURN", prefix);
    } else if (node->type == '+') {
        debug("%sOP: +", prefix);
    } else if (node->type == '-') {
        debug("%sOP: -", prefix);
    } else if (node->type == '>') {
        debug("%sOP: >", prefix);
    } else if (node->type == '<') {
        debug("%sOP: <", prefix);
    } else if (node->type == '*') {
        debug("%sOP: *", prefix);
    } else if (node->type == '/') {
        debug("%sOP: /", prefix);
    } else if (node->type == '=') {
        debug("%sOP: =", prefix);
    }
}


void debug_print_ast(Node *node, char *prefix) {
    print_ast_node(node, prefix);

    char new_prefix[100];
    snprintf(new_prefix, 100, "%s%s", "    ", prefix);

    if (node->lhs)
        debug_print_ast(node->lhs, new_prefix);

    if (node->rhs)
        debug_print_ast(node->rhs, new_prefix);
}


void print_ast(Node *node) {
    debug_print_ast(node, "");
}
