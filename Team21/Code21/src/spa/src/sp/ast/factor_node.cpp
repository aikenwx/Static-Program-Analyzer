#include "factor_node.h"

namespace ast {
FactorNode::FactorNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> FactorNode::GetOperand() {
  return operand;
}

std::string FactorNode::ToString() const {
  return "factor:" + operand->ToString() + "\n";
}
}
