#pragma once
#include "i_node.h"

namespace ast {
class FactorNode : public INode {
public:
  FactorNode(INode *operand);

  INode *GetOperand();
  std::ostream &Write(std::ostream &out) const override;

private:
  INode *operand;
};
}
