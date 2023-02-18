#include "relational_expression_node.h"

namespace ast {
RelationalExpressionNode::RelationalExpressionNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> RelationalExpressionNode::GetOperand() {
  return operand;
}

std::string RelationalExpressionNode::ToString() const {
  return "rel_expression:" + operand->ToString() + "\n";
}
}
