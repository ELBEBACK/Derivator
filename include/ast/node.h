#ifndef DRV_BTREE_NODE_H
#define DRV_BTREE_NODE_H


typedef enum Nodetype {
    NODE_NUM,
    NODE_VAR,
    NODE_OP,
    NODE_FUNC,
} NodeType;


typedef enum Variable {
    VAR_X
} Variable;

typedef enum Operation {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    UNDEFINED, 
} Operation;

typedef enum MathFunction {
    FUNC_LN,
    FUNC_EXP,
    FUNC_SIN,
    FUNC_COS,
    FUNC_TG,
    FUNC_CTG,
    FUNC_SH,
    FUNC_CH,
    FUNC_TH,
    FUNC_CTH,
    FUNC_ARCSIN,
    FUNC_ARCCOS,
    FUNC_ARCTG,
    FUNC_ARCCTG,
    FUNC_ARCSH,
    FUNC_ARCCH,
    FUNC_ARCTH,
    FUNC_ARCCTH,
} MathFunction;   


typedef union NodeValue {
    double          number;
    Variable        variable;
    Operation       op;
    MathFunction    func;
} NodeValue;


typedef struct Node {
    NodeType        type;
    NodeValue       value;
    struct Node*    left;
    struct Node*    right;
} Node;


#ifdef DEBUG
void node_print(const Node* node);
#endif // DEBUG
    

#endif // DRV_BTREE_NODE_H