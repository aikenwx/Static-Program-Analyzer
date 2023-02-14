#pragma once
#include "i_node.h"

namespace ast {
class FactorNode : public INode {
public:
  FactorNode(INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
