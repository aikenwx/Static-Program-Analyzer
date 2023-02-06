#pragma once
#include "i_node.h"

namespace ast {
class AST {
public:
  INode *GetRoot();

private:
  INode *root;
};
}
