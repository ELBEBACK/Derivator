#ifndef DRV_DUMP_H
#define DRV_DUMP_H


#include <stdio.h>
#include "ast/node.h"

int dump_graphviz(const char* dst_path, const Node* node);

#endif // DRV_DUMP_H 

