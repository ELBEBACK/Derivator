#include <stdio.h>

#include "ast/tree.h"   
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "derivator.h"  
#include "simplifier.h"
#include "output/dump.h"


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

    if (dump_graphviz("assets/dot/ast_original.dot", ast_root) != 0) {
        fprintf(stderr, "Graphviz dump fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        tree_destroy(ast_root);
        token_stream_destroy(&token_stream);
        return -1;
    }

    Node* derivative = expression_derive(ast_root);
    if (!derivative) {
        fprintf(stderr, "Derivation fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        tree_destroy(ast_root);
        token_stream_destroy(&token_stream);
        return -1;
    }

    if (dump_graphviz("assets/dot/ast_derivated_raw.dot", derivative) != 0) {
        fprintf(stderr, "Graphviz dump fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        tree_destroy(ast_root);
        token_stream_destroy(&token_stream);
        return -1;
    }

    Node* simplified = expression_simplify(derivative);
    if (!simplified) {
        fprintf(stderr, "Simplification fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        tree_destroy(ast_root);
        tree_destroy(derivative);
        token_stream_destroy(&token_stream);
        return -1;
    }

    if (dump_graphviz("assets/dot/ast_derivated.dot", simplified) != 0) {
        fprintf(stderr, "Graphviz dump fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        tree_destroy(ast_root);
        token_stream_destroy(&token_stream);
        return -1;
    }

    #ifdef DEBUG
        node_print(simplified);
        printf("\n");
    #endif


    tree_destroy(ast_root);
    token_stream_destroy(&token_stream);
    tree_destroy(derivative);
    tree_destroy(simplified);

    return 0;
}
