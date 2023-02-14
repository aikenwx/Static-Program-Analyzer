#pragma once
#include "i_node.h"

namespace ast {
class BinaryOperationNode : public INode {
public:
  BinaryOperationNode(INode *left, INode *right);

  INode *GetLeft();
  INode *GetRight();
  virtual std::string ToString() const override = 0;

protected:
  INode *left;
  INode *right;
};
}
