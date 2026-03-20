#ifdef DEBUG
#include <stdio.h>

#include "ast/node.h"


void node_print(const Node* node) {
    if (!node) {
        printf("NULL");
        return;
    }

    switch (node->type) {
        case NODE_NUM:
            printf("%g", node->value.number);
            break;

        case NODE_VAR:
            printf("x");
            break;

        case NODE_OP:
            printf("(");
            node_print(node->left);
            switch (node->value.op) {
                case OP_ADD: printf(" + "); break;
                case OP_SUB: printf(" - "); break;
                case OP_MUL: printf(" * "); break;
                case OP_DIV: printf(" / "); break;
                case OP_POW: printf(" ^ "); break;
                default:     printf(" ? "); break;
            }
            node_print(node->right);
            printf(")");
            break;

        case NODE_FUNC:
            switch (node->value.func) {
                case FUNC_LN:     printf("ln");     break;
                case FUNC_EXP:    printf("exp");    break;
                case FUNC_SIN:    printf("sin");    break;
                case FUNC_COS:    printf("cos");    break;
                case FUNC_TG:     printf("tg");     break;
                case FUNC_CTG:    printf("ctg");    break;
                case FUNC_SH:     printf("sh");     break;
                case FUNC_CH:     printf("ch");     break;
                case FUNC_TH:     printf("th");     break;
                case FUNC_CTH:    printf("cth");    break;
                case FUNC_ARCSIN: printf("arcsin"); break;
                case FUNC_ARCCOS: printf("arccos"); break;
                case FUNC_ARCTG:  printf("arctg");  break;
                case FUNC_ARCCTG: printf("arcctg"); break;
                case FUNC_ARCSH:  printf("arcsh");  break;
                case FUNC_ARCCH:  printf("arcch");  break;
                case FUNC_ARCTH:  printf("arcth");  break;
                case FUNC_ARCCTH: printf("arccth"); break;
                default:          printf("?func");  break;
            }
            printf("(");
            node_print(node->right);   // arg is always in ->right
            printf(")");
            break;

        default:
            printf("(unknown node type %d)", node->type);
            break;
    }
}
#endif // DEBUG

