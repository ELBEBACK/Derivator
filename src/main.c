#include <stdio.h>

#include "ast/tree.h"
#include "ast/node_macros.h"
#include "ast/node.h" 
#include "output/dump.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"


int main (const int argc, const char** argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return -1;
    }

    TokenStream token_stream = {0};
    {
        int bufsize = 1024;
        if (lexer_tokenize(&token_stream, argv, bufsize) != 0) {
            fprintf(stderr, "Tokenization fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
            return -1;
        }
    }

    Node* ast_root = expression_parse(&token_stream);
    if (!ast_root) {
        fprintf(stderr, "Parsing fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    #ifdef DEBUG
        node_print(ast_root);
        printf("\n");
    #endif

    if (dump_graphviz("assets/ast_original.dot", ast_root) != 0) {
        fprintf(stderr, "Graphviz dump fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        tree_destroy(ast_root);
        token_stream_destroy(&token_stream);
        return -1;
    }

    tree_destroy(ast_root);
    token_stream_destroy(&token_stream);

    return 0;
}
