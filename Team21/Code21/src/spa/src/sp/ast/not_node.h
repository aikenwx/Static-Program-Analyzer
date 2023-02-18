#pragma once
#include "i_node.h"

namespace ast {
class NotNode : public INode {
public:
  NotNode(INode *operand);

  INode *GetOperand();
  std::string ToString() const override;

private:
  INode *operand;
};
}
