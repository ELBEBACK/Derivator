#ifndef DRV_SIMPLIFIER_H
#define DRV_SIMPLIFIER_H

#include <math.h>

#include "ast/node.h"


Node* expression_simplify(const Node* expr);

#endif // DRV_SIMPLIFIER_H