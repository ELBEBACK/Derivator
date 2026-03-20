#include <stdio.h>
#include <stdlib.h>

#include "ast/node.h"
#include "output/dump.h"

#define COLOR_BG    "#1E1B2E"
#define COLOR_TEXT  "#4E2859"
#define COLOR_ARROW "#F4EFFF"

#define COLOR_NUM   "#B967FF"   
#define COLOR_VAR   "#9852CB"   
#define COLOR_OP    "#E8C9F7"   
#define COLOR_FUNC  "#FF71CE"
#define COLOR_UNDEF "#E74C3C"   


static void write_node_block(FILE* f, const Node* node);
static const char* func_label(MathFunction f);
static const char* op_label(Operation op);



int dump_graphviz(const char* dst_path, const Node* root) {

    FILE* output_file = fopen(dst_path, "w");
    if (!output_file) {
        fprintf(stderr, "Could not open file '%s'\n", dst_path);
        return -1;
    }

    fprintf(output_file, "digraph AST {\n");
    fprintf(output_file, "  graph [rankdir=TB splines=true bgcolor=\"%s\"];\n", COLOR_BG);
    fprintf(output_file, "  node  [fontname=\"Courier\", fontsize=16, margin=0.3, color=\"transparent\", fontcolor=\"%s\"];\n", COLOR_TEXT);
    fprintf(output_file, "  edge [arrowhead=normal, penwidth=2, arrowsize=0.5, color=\"%s\"]\n\n", COLOR_ARROW);
    fprintf(output_file, "\n");

    if (!root) {
        fprintf(output_file, "  empty [label=\"(empty tree)\" shape=plaintext];\n");
    } else {
        write_node_block(output_file, root);
    }

    fprintf(output_file, "}\n");
    fclose(output_file);

    return 0;
}


static void write_node_block(FILE* output_file, const Node* node) {

    if (!node) return;

    unsigned long id = (unsigned long)(void*) node;

    switch (node->type) {

        case NODE_NUM:
            fprintf(output_file, "  n%lu [label=\"%g\" shape=ellipse style=filled fillcolor=\"%s\"];\n", id, node->value.number, COLOR_NUM);
            break;

        case NODE_VAR:
            fprintf(output_file, "  n%lu [label=\"x\" shape=ellipse style=filled fillcolor=\"%s\"];\n", id, COLOR_VAR);
            break;

        case NODE_OP:
            fprintf(output_file, "  n%lu [label=\"%s\" shape=circle style=filled fillcolor=\"%s\"];\n", id, op_label(node->value.op), COLOR_OP);
            break;

        case NODE_FUNC:
            fprintf(output_file, "  n%lu [label=\"%s\" shape=box style=\"filled,rounded\" fillcolor=\"%s\"];\n", id, func_label(node->value.func), COLOR_FUNC);
            break;

        default:
            fprintf(output_file, "  n%lu [label=\"?\" shape=diamond style=filled fillcolor=\"%s\" color=\"%s\"];\n", id, COLOR_UNDEF, COLOR_UNDEF);
            break;
    }

    if (node->left) {
        write_node_block(output_file, node->left);
        fprintf(output_file, "  n%lu -> n%lu \n", id, (unsigned long)(void*) node->left);
    }

    if (node->right) {
        write_node_block(output_file, node->right);
        fprintf(output_file, "  n%lu -> n%lu \n", id, (unsigned long)(void*) node->right);
    }
}


static const char* func_label(MathFunction math_func) {
    switch (math_func) {
        case FUNC_LN:     return "ln";
        case FUNC_EXP:    return "exp";
        case FUNC_SIN:    return "sin";
        case FUNC_COS:    return "cos";
        case FUNC_TG:     return "tg";
        case FUNC_CTG:    return "ctg";
        case FUNC_SH:     return "sh";
        case FUNC_CH:     return "ch";
        case FUNC_TH:     return "th";
        case FUNC_CTH:    return "cth";
        case FUNC_ARCSIN: return "arcsin";
        case FUNC_ARCCOS: return "arccos";
        case FUNC_ARCTG:  return "arctg";
        case FUNC_ARCCTG: return "arcctg";
        case FUNC_ARCSH:  return "arcsh";
        case FUNC_ARCCH:  return "arcch";
        case FUNC_ARCTH:  return "arcth";
        case FUNC_ARCCTH: return "arccth";
        default:          return "?";
    }
}


static const char* op_label(Operation op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "−";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_POW: return "^";
        default:     return "?";
    }
}