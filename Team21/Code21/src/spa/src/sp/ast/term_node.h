#pragma once
#include "i_node.h"

namespace ast {
class TermNode : public INode {
public:
  TermNode(INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
