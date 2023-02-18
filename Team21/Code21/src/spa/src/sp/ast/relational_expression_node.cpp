#include "relational_expression_node.h"

namespace ast {
RelationalExpressionNode::RelationalExpressionNode(INode *operand) {
  this->operand = operand;
}

INode *RelationalExpressionNode::GetOperand() {
  return operand;
}

std::string RelationalExpressionNode::ToString() const {
  return "rel_expression:" + operand->ToString() + "\n";
}
}
