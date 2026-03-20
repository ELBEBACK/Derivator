#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>

#include "ast/tree.h"


Node* node_create(NodeType type, NodeValue value, Node* left, Node* right) {
    
    Node* node_to_create = (Node*) calloc(1, sizeof(Node));
    if (!node_to_create) {
        #ifdef DEBUG
        fprintf(stderr, "Allocation fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        #endif
        return NULL;
    }

    node_to_create->type = type;
    node_to_create->value = value;
    node_to_create->left = left;
    node_to_create->right = right;
    
    return node_to_create;
}



void tree_destroy(Node* root) {
    
    if (!root) return;

    tree_destroy(root->left);
    tree_destroy(root->right);
    free(root);
}



Node* tree_copy(const Node* root) {
    
    if (!root) return NULL;

    Node* left_copy = tree_copy(root->left);
    Node* right_copy = tree_copy(root->right);

    Node* root_copy = node_create(root->type, root->value, left_copy, right_copy);
    if (!root_copy) {

        #ifdef DEBUG
        fprintf(stderr, "Copy fail in %s at %s:%d\n", __func__, __FILE__, __LINE__);
        #endif

        tree_destroy(left_copy);
        tree_destroy(right_copy);
        return NULL;
    }

    return root_copy;
}
