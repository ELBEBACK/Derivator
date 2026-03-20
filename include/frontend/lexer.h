#ifndef DRV_LEXER_H
#define DRV_LEXER_H

#include "ast/node.h"


typedef enum TokenType {
    TOKEN_NUM,
    TOKEN_VAR,
    TOKEN_OP,
    TOKEN_FUNC,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END_OF_EXPRESSION,
} TokenType;


typedef struct Token {
    TokenType       type;
    NodeValue       value;
} Token;


typedef struct TokenStream {
    Token* tokens;
    int position;
    int count;
} TokenStream;


int lexer_tokenize(TokenStream* stream, const char** argv, int size);
void token_stream_destroy(TokenStream* stream);


#endif // DRV_LEXER_H