#include "expression_node.h"

namespace ast {
ExpressionNode::ExpressionNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> ExpressionNode::GetOperand() {
  return operand;
}

std::string ExpressionNode::ToString() const {
  return "expression:" + operand->ToString() + "\n";
}
}
