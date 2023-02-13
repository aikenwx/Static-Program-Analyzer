#pragma once
#include "i_node.h"

namespace ast {
class TermNode : public INode {
public:
  TermNode(INode *operand);

  INode *GetOperand();
  std::ostream &Write(std::ostream &out) const override;

private:
  INode *operand;
};
}
