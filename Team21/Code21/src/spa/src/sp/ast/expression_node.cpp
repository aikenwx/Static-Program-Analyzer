#include "expression_node.h"

namespace ast {
ExpressionNode::ExpressionNode(INode *operand) {
  this->operand = operand;
}

INode *ExpressionNode::GetOperand() {
  return operand;
}

std::string ExpressionNode::ToString() const {
  return "expression:" + operand->ToString() + "\n";
}
}
