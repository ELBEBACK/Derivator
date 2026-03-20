#ifndef DRV_BTREE_H
#define DRV_BTREE_H


#include "ast/node.h"

Node* node_create(NodeType type, NodeValue value, Node* left, Node* right);

void tree_destroy(Node* root);
Node* tree_copy(const Node* root);


#endif // DRV_BTREE_H