#include "factor_node.h"

namespace ast {
FactorNode::FactorNode(INode *operand) {
  this->operand = operand;
}

INode *FactorNode::GetOperand() {
  return operand;
}

std::string FactorNode::ToString() const {
  return "factor:" + operand->ToString() + "\n";
}
}
