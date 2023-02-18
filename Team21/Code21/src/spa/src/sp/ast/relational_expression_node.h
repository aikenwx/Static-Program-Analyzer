#pragma once
#include "i_node.h"

namespace ast {
class RelationalExpressionNode : public INode {
public:
  RelationalExpressionNode (INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
