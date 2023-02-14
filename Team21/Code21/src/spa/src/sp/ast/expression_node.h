#pragma once
#include "i_node.h"

namespace ast {
class ExpressionNode : public INode {
public:
  ExpressionNode(INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
