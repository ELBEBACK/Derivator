#ifdef DEBUG
#include <stdio.h>
#endif

#include <stdlib.h>
#include <math.h>

#include "simplifier.h"
#include "ast/node_macros.h"
#include "ast/tree.h"



static int is_num(const Node* n)              { return n && n->type == NODE_NUM; }
static int is_num_val(const Node* n, double v) { return is_num(n) && n->value.number == v; }

static Node* simplify_once(const Node* node);
static int count_nodes(const Node* n); 


Node* expression_simplify(const Node* expr) {
    if (!expr) {
        #ifdef DEBUG
        fprintf(stderr, "Null pointer was sent to %s at %s:%d\n", __func__, __FILE__, __LINE__);
        #endif
        return NULL;
    }

    Node* current = tree_copy(expr);

    while(1) {
        int before = count_nodes(current);
        Node* next = simplify_once(current);
        tree_destroy(current);
        current = next;
        int after = count_nodes(current);

        if (after >= before) break;
    }

    return current;
}

static Node* simplify_once(const Node* node) {

    if (!node) return NULL;

    if (node->type == NODE_NUM || node->type == NODE_VAR)
        return tree_copy(node);

    Node* l = simplify_once(node->left);
    Node* r = simplify_once(node->right);

    if (node->type == NODE_FUNC) {
        NodeValue val = {.func = node->value.func};
        return node_create(NODE_FUNC, val, NULL, r);
    }

    switch (node->value.op) {

        case OP_ADD: {
            
            if (is_num_val(l, 0)) { tree_destroy(l); return r; }
            if (is_num_val(r, 0)) { tree_destroy(r); return l; }
            
            if (is_num(l) && is_num(r)) {
                double v = l->value.number + r->value.number;
                tree_destroy(l); tree_destroy(r);
                return NUM(v);
            }
            break;
        }

        case OP_SUB: {
            
            if (is_num_val(r, 0)) { tree_destroy(r); return l; }
            if (is_num_val(l, 0) && is_num_val(r, 0)) {
                tree_destroy(l); tree_destroy(r);
                return NUM(0);
            }
            
            if (is_num(l) && is_num(r)) {
                double v = l->value.number - r->value.number;
                tree_destroy(l); tree_destroy(r);
                return NUM(v);
            }
            break;
        }

        case OP_MUL: {
            
            if (is_num_val(l, 0)) { tree_destroy(l); tree_destroy(r); return NUM(0); }
            if (is_num_val(r, 0)) { tree_destroy(l); tree_destroy(r); return NUM(0); }
            
            if (is_num_val(l, 1)) { tree_destroy(l); return r; }
            if (is_num_val(r, 1)) { tree_destroy(r); return l; }
            
            if (is_num(l) && is_num(r)) {
                double v = l->value.number * r->value.number;
                tree_destroy(l); tree_destroy(r);
                return NUM(v);
            }
            break;
        }

        case OP_DIV: {
            
            if (is_num_val(l, 0)) { tree_destroy(l); tree_destroy(r); return NUM(0); }
            if (is_num_val(r, 1)) { tree_destroy(r); return l; }
            
            if (is_num(l) && is_num(r) && r->value.number != 0) {
                double v = l->value.number / r->value.number;
                tree_destroy(l); tree_destroy(r);
                return NUM(v);
            }
            break;
        }

        case OP_POW: {
            
            if (is_num_val(r, 0)) { tree_destroy(l); tree_destroy(r); return NUM(1); }
            if (is_num_val(r, 1)) { tree_destroy(r); return l; }
            
            if (is_num_val(l, 0)) { tree_destroy(l); tree_destroy(r); return NUM(0); }
            if (is_num_val(l, 1)) { tree_destroy(l); tree_destroy(r); return NUM(1); }
            
            if (is_num(l) && is_num(r)) {
                double v = pow(l->value.number, r->value.number);
                tree_destroy(l); tree_destroy(r);
                return NUM(v);
            }
            break;
        }

        default: break;
    }

    NodeValue val = {.op = node->value.op};
    return node_create(NODE_OP, val, l, r);
}


static int count_nodes(const Node* n) {
    if (!n) return 0;
    return 1 + count_nodes(n->left) + count_nodes(n->right);
}

