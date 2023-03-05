#include "unary_operation_node.h"

namespace ast {
UnaryOperationNode::UnaryOperationNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> UnaryOperationNode::GetOperand() { return operand; }

void UnaryOperationNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  // nb: extractor doesn't have a HandleUnaryOperationNode method
  // call HandleOtherNode() to let extractor update state
  // e.g. node parents
  extractor->HandleOtherNode(currentNode, depth);

  operand->AcceptVisitor(operand, extractor, depth + 1);
}
}  // namespace ast
