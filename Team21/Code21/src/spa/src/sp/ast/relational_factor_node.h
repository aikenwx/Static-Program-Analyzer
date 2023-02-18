#pragma once
#include "i_node.h"

namespace ast {
class RelationalFactorNode : public INode {
public:
  RelationalFactorNode(INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
