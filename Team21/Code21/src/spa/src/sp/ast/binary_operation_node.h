#pragma once
#include "i_node.h"

namespace ast {
class BinaryOperationNode : public INode {
public:
  BinaryOperationNode(INode *left, INode *right);

private:
  INode *left;
  INode *right;
};
}
