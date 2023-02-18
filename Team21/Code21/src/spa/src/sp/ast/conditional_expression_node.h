#pragma once
#include "i_node.h"

namespace ast {
class ConditionalExpressionNode : public INode {
public:
  ConditionalExpressionNode(INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
