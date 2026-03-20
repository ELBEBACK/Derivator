#ifndef DRV_NODE_MACROS_H
#define DRV_NODE_MACROS_H

#include "ast/tree.h"


#define NUM(num_val)         \
    node_create(NODE_NUM, (NodeValue) {.number = (num_val)}, NULL, NULL)

#define VAR()               \
    node_create(NODE_VAR, (NodeValue) {.variable = VAR_X}, NULL, NULL)


#define ADD(left, right)    \
    node_create(NODE_OP, (NodeValue) {.op = OP_ADD}, (left), (right))

#define SUB(left, right)    \
    node_create(NODE_OP, (NodeValue) {.op = OP_SUB}, (left), (right))

#define MUL(left, right)    \
    node_create(NODE_OP, (NodeValue) {.op = OP_MUL}, (left), (right))

#define DIV(left, right)    \
    node_create(NODE_OP, (NodeValue) {.op = OP_DIV}, (left), (right))

#define POW(base, exp)      \
    node_create(NODE_OP, (NodeValue) {.op = OP_POW}, (base), NUM(exp))


#define EXP(arg)            \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_EXP}, NULL, (arg))

#define LN(arg)             \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_LN}, NULL, (arg))

#define SIN(arg)            \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_SIN}, NULL, (arg))

#define COS(arg)            \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_COS}, NULL, (arg))

#define TG(arg)             \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_TG}, NULL, (arg))

#define CTG(arg)            \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_CTG}, NULL, (arg))  

#define SH(arg)             \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_SH}, NULL, (arg))

#define CH(arg)             \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_CH}, NULL, (arg))

#define TH(arg)             \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_TH}, NULL, (arg))

#define CTH(arg)            \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_CTH}, NULL, (arg))

#define ARCSIN(arg)         \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCSIN}, NULL, (arg))

#define ARCCOS(arg)         \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCCOS}, NULL, (arg))

#define ARCTG(arg)          \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCTG}, NULL, (arg))

#define ARCCTG(arg)         \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCCTG}, NULL, (arg))

#define ARCSH(arg)         \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCSH}, NULL, (arg))

#define ARCCH(arg)         \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCCH}, NULL, (arg))

#define ARCTH(arg)         \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCTH}, NULL, (arg))

#define ARCCTH(arg)        \
    node_create(NODE_FUNC, (NodeValue){.func = FUNC_ARCCTH}, NULL, (arg))  

#endif // DRV_NODE_MACROS_H