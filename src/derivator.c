#include <stdio.h>
#include <stdlib.h>

#include "derivator.h"
#include "ast/node_macros.h"
#include "ast/tree.h"


#define COPY(n)  tree_copy(n)
#define DRV(n)     expression_derive(n)


Node* expression_derive(const Node* expr) {
    if (!expr) {
        #ifdef DEBUG
        fprintf(stderr, "Null pointer was sent to %s at %s:%d\n", __func__, __FILE__, __LINE__);
        #endif
        return NULL;
    }

    switch (expr->type) {

        case NODE_NUM:
            return NUM(0);

        case NODE_VAR:
            return NUM(1);

        case NODE_OP: {
            Node* l = expr->left;
            Node* r = expr->right;
            switch (expr->value.op) {

                case OP_ADD: return ADD(DRV(l), DRV(r));

                case OP_SUB: return SUB(DRV(l), DRV(r));

                case OP_MUL:
                    return ADD(MUL(DRV(l), COPY(r)),
                               MUL(COPY(l), DRV(r)));

                case OP_DIV:
                    return DIV(SUB(MUL(DRV(l), COPY(r)),
                                   MUL(COPY(l), DRV(r))),
                               POW(COPY(r), 2));

                case OP_POW: {
                    if (r->type == NODE_NUM) {
                        double c = r->value.number;
                        return MUL(MUL(NUM(c), POW(COPY(l), c - 1)), DRV(l));
                    }
                    return MUL(
                        node_create(NODE_OP, (NodeValue){.op=OP_POW}, COPY(l), COPY(r)),
                        ADD(MUL(DRV(r), LN(COPY(l))),
                            MUL(COPY(r), DIV(DRV(l), COPY(l))))
                    );
                }
                default: break;
            }
            break;
        }

        case NODE_FUNC: {
            Node* arg = expr->right; 
            switch (expr->value.func) {

                case FUNC_LN:   return DIV(DRV(arg), COPY(arg));

                case FUNC_EXP:  return MUL(EXP(COPY(arg)), DRV(arg));

                case FUNC_SIN:  return MUL(COS(COPY(arg)), DRV(arg));

                case FUNC_COS:  return MUL(SUB(NUM(0), SIN(COPY(arg))), DRV(arg));

                case FUNC_TG:
                    return DIV(DRV(arg), POW(COS(COPY(arg)), 2));

                case FUNC_CTG:
                    return DIV(SUB(NUM(0), DRV(arg)), POW(SIN(COPY(arg)), 2));

                case FUNC_SH:   return MUL(CH(COPY(arg)), DRV(arg));

                case FUNC_CH:   return MUL(SH(COPY(arg)), DRV(arg));

                case FUNC_TH:
                    return DIV(DRV(arg), POW(CH(COPY(arg)), 2));

                case FUNC_CTH:
                    return DIV(SUB(NUM(0), DRV(arg)), POW(SH(COPY(arg)), 2));

                case FUNC_ARCSIN:
                    return DIV(DRV(arg),
                        POW(SUB(NUM(1), POW(COPY(arg), 2)), 0.5));

                case FUNC_ARCCOS:
                    return DIV(SUB(NUM(0), DRV(arg)),
                        POW(SUB(NUM(1), POW(COPY(arg), 2)), 0.5));

                case FUNC_ARCTG:
                    return DIV(DRV(arg), ADD(NUM(1), POW(COPY(arg), 2)));

                case FUNC_ARCCTG:
                    return DIV(SUB(NUM(0), DRV(arg)),
                               ADD(NUM(1), POW(COPY(arg), 2)));

                case FUNC_ARCSH:
                    return DIV(DRV(arg),
                        POW(ADD(POW(COPY(arg), 2), NUM(1)), 0.5));

                case FUNC_ARCCH:
                    return DIV(DRV(arg),
                        POW(SUB(POW(COPY(arg), 2), NUM(1)), 0.5));

                case FUNC_ARCTH:
                case FUNC_ARCCTH:
                    return DIV(DRV(arg), SUB(NUM(1), POW(COPY(arg), 2)));

                default: break;
            }
            break;
        }
        default: break;
    }

    fprintf(stderr, "Derivator: node is not supported\n");
    return NULL;
}