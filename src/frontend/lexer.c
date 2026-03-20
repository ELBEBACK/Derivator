#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "frontend/lexer.h"


#define TOKENIZE_NUM(num_val, offset)                                                       \
    dst_stream->tokens[dst_stream->count].type              = TOKEN_NUM;                    \
    dst_stream->tokens[dst_stream->count].value.number      = num_val;                      \
    ++dst_stream->count;                                                                    \
    dst_stream->position += offset;

#define TOKENIZE_VAR(var, offset)                                                           \
    dst_stream->tokens[dst_stream->count].type              = TOKEN_VAR;                    \
    dst_stream->tokens[dst_stream->count].value.variable    = var;                          \
    ++dst_stream->count;                                                                    \
    dst_stream->position += offset;

#define TOKENIZE_OP(optype, offset)                                                         \
    dst_stream->tokens[dst_stream->count].type              = TOKEN_OP;                     \
    dst_stream->tokens[dst_stream->count].value.op          = optype;                       \
    ++dst_stream->count;                                                                    \
    dst_stream->position += offset;

#define TOKENIZE_FUNC(math_func, offset)                                                    \
    dst_stream->tokens[dst_stream->count].type              = TOKEN_FUNC;                   \
    dst_stream->tokens[dst_stream->count].value.func        = math_func;                    \
    ++dst_stream->count;                                                                    \
    dst_stream->position += offset;

#define TOKENIZE_LEXEME(lexeme, offset)                                                     \
    dst_stream->tokens[dst_stream->count].type              = lexeme;                       \
    dst_stream->tokens[dst_stream->count].value.op          = UNDEFINED;                    \
    ++dst_stream->count;                                                                    \
    dst_stream->position += offset;



static void tokenize_expression(TokenStream* dst_stream, const char* expr_buffer, int size);


int lexer_tokenize(TokenStream* dst_stream, const char** argv, int size) {

    FILE* input_file = fopen(argv[1], "rb");
    if (!input_file) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return -1;
    }

    dst_stream->tokens = (Token*) calloc(size, sizeof(Node));
    if (!dst_stream->tokens) {
        #ifdef DEBUG
        fprintf(stderr, "Allocation fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        #endif
        fclose(input_file);
        return -1; 
    }

    char* expr_buffer = (char*) calloc(size, sizeof(char));
    if (!expr_buffer) {
        #ifdef DEBUG
        fprintf(stderr, "Allocation fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        #endif
        free(dst_stream->tokens);
        fclose(input_file);
        return -1; 
    }

    {
        int read_bytes = fread(expr_buffer, sizeof(char), size, input_file);
        if (read_bytes < 0) {
            fprintf(stderr, "Error reading file %s\n", argv[1]);
            free(expr_buffer);
            free(dst_stream->tokens);
            fclose(input_file);
            return -1;
        }
    }

    tokenize_expression(dst_stream, expr_buffer, size);
    
    dst_stream->position = 0;

    return 0;
}



static void tokenize_expression(TokenStream* dst_stream, const char* expr_buffer, int size) {
    
    assert(dst_stream);
    assert(expr_buffer);

    while (dst_stream->position < size) {
        
        while (expr_buffer[dst_stream->position] == ' ') ++dst_stream->position;

        if (isdigit(expr_buffer[dst_stream->position])) {
            double num_value = 0;
            int count = 0;

            sscanf(&expr_buffer[dst_stream->position], "%lg%n", &num_value, &count);
            TOKENIZE_NUM(num_value, count);
        }

        else if (expr_buffer[dst_stream->position] == 'x') { TOKENIZE_VAR(VAR_X, 1); }

        else if (expr_buffer[dst_stream->position] == '(') { TOKENIZE_LEXEME(TOKEN_LPAREN, 1); }
        else if (expr_buffer[dst_stream->position] == ')') { TOKENIZE_LEXEME(TOKEN_RPAREN, 1); }

        else if (expr_buffer[dst_stream->position] == '+') { TOKENIZE_OP(OP_ADD, 1); } 
        else if (expr_buffer[dst_stream->position] == '-') { TOKENIZE_OP(OP_SUB, 1); }
        else if (expr_buffer[dst_stream->position] == '*') { TOKENIZE_OP(OP_MUL, 1); }
        else if (expr_buffer[dst_stream->position] == '/') { TOKENIZE_OP(OP_DIV, 1); }
        else if (expr_buffer[dst_stream->position] == '^') { TOKENIZE_OP(OP_POW, 1); } 
        
        else if(expr_buffer[dst_stream->position] == '$') {
            TOKENIZE_LEXEME(TOKEN_END_OF_EXPRESSION, 1);
            return;
        }

        else if (expr_buffer[dst_stream->position] == 'l' && expr_buffer[dst_stream->position + 1] == 'n') { TOKENIZE_FUNC(FUNC_LN, 2); } else if (expr_buffer[dst_stream->position] == 'e' && expr_buffer[dst_stream->position + 1] == 'x' && expr_buffer[dst_stream->position + 2] == 'p') {
                TOKENIZE_FUNC(FUNC_EXP, 3);

        } else if (expr_buffer[dst_stream->position] == 's' && expr_buffer[dst_stream->position + 1] == 'i' && expr_buffer[dst_stream->position + 2] == 'n') {
                TOKENIZE_FUNC(FUNC_SIN, 3); 
        
        } else if (expr_buffer[dst_stream->position] == 'c' && expr_buffer[dst_stream->position + 1] == 'o' && expr_buffer[dst_stream->position + 2] == 's') {
                TOKENIZE_FUNC(FUNC_COS, 3);
        
        } else if (expr_buffer[dst_stream->position] == 't' && expr_buffer[dst_stream->position + 1] == 'g') {
                TOKENIZE_FUNC(FUNC_TG, 2);
        
        } else if (expr_buffer[dst_stream->position] == 'c' && expr_buffer[dst_stream->position + 1] == 't' && expr_buffer[dst_stream->position + 2] == 'g') {
                TOKENIZE_FUNC(FUNC_CTG, 3);
        
        } else if (expr_buffer[dst_stream->position] == 's' && expr_buffer[dst_stream->position + 1] == 'h') {
                TOKENIZE_FUNC(FUNC_SH, 2);
        
        } else if (expr_buffer[dst_stream->position] == 'c' && expr_buffer[dst_stream->position + 1] == 'h') {
                TOKENIZE_FUNC(FUNC_CH, 2);
    
        } else if (expr_buffer[dst_stream->position] == 't' && expr_buffer[dst_stream->position + 1] == 'h') { 
                TOKENIZE_FUNC(FUNC_TH, 2);
        
        } else if (expr_buffer[dst_stream->position] == 'c' && expr_buffer[dst_stream->position + 1] == 't' && expr_buffer[dst_stream->position + 2] == 'h') {
                TOKENIZE_FUNC(FUNC_CTH, 3);
        
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 's' && expr_buffer[dst_stream->position + 4] == 'i' && expr_buffer[dst_stream->position + 5] == 'n') {
                TOKENIZE_FUNC(FUNC_ARCSIN, 6);
        
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 'c' && expr_buffer[dst_stream->position + 4] == 'o' && expr_buffer[dst_stream->position + 5] == 's') {
                TOKENIZE_FUNC(FUNC_ARCCOS, 6);
        
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 't' && expr_buffer[dst_stream->position + 4] == 'g') {
                TOKENIZE_FUNC(FUNC_ARCTG, 5);
    
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 'c' && expr_buffer[dst_stream->position + 4] == 't' && expr_buffer[dst_stream->position + 5] == 'g') {
                TOKENIZE_FUNC(FUNC_ARCCTG, 6);
        
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 's' && expr_buffer[dst_stream->position + 4] == 'h') {
                TOKENIZE_FUNC(FUNC_ARCSH, 5);
        
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 'c' && expr_buffer[dst_stream->position + 4] == 'h') {
                TOKENIZE_FUNC(FUNC_ARCCH, 5);
    
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 't' && expr_buffer[dst_stream->position + 4] == 'h') {
                TOKENIZE_FUNC(FUNC_ARCTH, 5);
        
        } else if (expr_buffer[dst_stream->position] == 'a' && expr_buffer[dst_stream->position + 1] == 'r' && expr_buffer[dst_stream->position + 2] == 'c' && expr_buffer[dst_stream->position + 3] == 'c' && expr_buffer[dst_stream->position + 4] == 't' && expr_buffer[dst_stream->position + 5] == 'h') {
                TOKENIZE_FUNC(FUNC_ARCCTH, 6);
        } else {
            fprintf(stderr, "Undefined character '%c' at position %d\n", expr_buffer[dst_stream->position], dst_stream->position);
            exit(1);
        }
    }

    return;
}



void token_stream_destroy(TokenStream* stream) {
    if (!stream) return;

    free(stream->tokens);
    stream->tokens = NULL;
    stream->position = 0;
    stream->count = 0;
}
