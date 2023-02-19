#include "unary_operation_node.h"

namespace ast {
UnaryOperationNode::UnaryOperationNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> UnaryOperationNode::GetOperand() {
  return operand;
}
}
