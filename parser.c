#include "kzcc.h"

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
    return NULL;
}

