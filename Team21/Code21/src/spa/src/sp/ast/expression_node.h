#pragma once
#include "i_node.h"

namespace ast {
class ExpressionNode : public INode {
public:
  ExpressionNode(INode *operand);

  INode *GetOperand();
  std::ostream &Write(std::ostream &out) const override;

private:
  INode *operand;
};
}
