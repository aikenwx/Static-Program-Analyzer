#include "expression_node.h"

namespace ast {
ExpressionNode::ExpressionNode(INode *operand) {
  this->operand = operand;
}

INode *ExpressionNode::GetOperand() {
  return operand;
}

std::ostream &ExpressionNode::Write(std::ostream &out) const {
  return out;
}
}
