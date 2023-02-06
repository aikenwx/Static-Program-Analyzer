#pragma once
#include "i_node.h"

namespace ast {
class AST {
public:
  INode *GetRoot();
  void SetRoot(INode *node);

private:
  INode *root;
};
}
