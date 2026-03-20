#include <stdio.h>
#include <stdlib.h>

#include "ast/node_macros.h"
#include "frontend/lexer.h"


static Node* parse_expr     (TokenStream* stream);
static Node* parse_term     (TokenStream* stream);
static Node* parse_factor   (TokenStream* stream);
static Node* parse_unary    (TokenStream* stream);
static Node* parse_power    (TokenStream* stream);
static Node* parse_primary  (TokenStream* stream);


static Token cur(TokenStream* stream)                    { return stream->tokens[stream->position]; }
static Token consume(TokenStream* stream)                { return stream->tokens[stream->position++]; }
static int   is_op(TokenStream* stream, Operation op)    { return cur(stream).type == TOKEN_OP && cur(stream).value.op == op; }


Node* expression_parse(TokenStream* stream) {
    Node* result = parse_expr(stream);
    return result;
}


static Node* parse_expr(TokenStream* stream) {
    Node* node = parse_term(stream);
    
    while (is_op(stream, OP_ADD) || is_op(stream, OP_SUB)) {
        Operation op = consume(stream).value.op;
        Node* right = parse_term(stream);
        NodeValue val = {.op = op};
        node = node_create(NODE_OP, val, node, right);
    }
    return node;
}


static Node* parse_term(TokenStream* stream) {
    Node* node = parse_power(stream);
    
    while (is_op(stream, OP_MUL) || is_op(stream, OP_DIV)) {
        Operation op = consume(stream).value.op;
        Node* right = parse_power(stream);
        NodeValue val = {.op = op};
        node = node_create(NODE_OP, val, node, right);
    }
    return node;
}


static Node* parse_power(TokenStream* stream) {
    Node* base = parse_unary(stream);
    if (is_op(stream, OP_POW)) {
        consume(stream);
        Node* exp = parse_power(stream);   
        NodeValue val = {.op = OP_POW};
        return node_create(NODE_OP, val, base, exp);
    }
    return base;
}


static Node* parse_unary(TokenStream* stream) {
    if (is_op(stream, OP_SUB)) {
        consume(stream);
        NodeValue val = {.op = OP_SUB};
        return node_create(NODE_OP, val, NUM(0), parse_unary(stream));
    }
    return parse_primary(stream);
}


static Node* parse_primary(TokenStream* stream) {
    Token primary_token = cur(stream);

    if (primary_token.type == TOKEN_NUM) {
        consume(stream);
        return NUM(primary_token.value.number);
    }

    if (primary_token.type == TOKEN_VAR) {
        consume(stream);
        return VAR();
    }

    if (primary_token.type == TOKEN_FUNC) {
        consume(stream);
        if (cur(stream).type != TOKEN_LPAREN) {
            fprintf(stderr, "Expected '(' after function usage\n"); 
            exit(1);
        }

        consume(stream);
        Node* arg = parse_expr(stream);
        if (cur(stream).type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')' after function argument\n"); 
            exit(1);
        }
        
        consume(stream);
        NodeValue val = {.func = primary_token.value.func};
        return node_create(NODE_FUNC, val, NULL, arg);
    }

    if (primary_token.type == TOKEN_LPAREN) {
        consume(stream);
        Node* node = parse_expr(stream);
        
        if (cur(stream).type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')'\n"); 
            exit(1);
        }

        consume(stream);
        return node;
    }

    fprintf(stderr, "Unexpected token type %d\n", primary_token.type);
    exit(1);
}