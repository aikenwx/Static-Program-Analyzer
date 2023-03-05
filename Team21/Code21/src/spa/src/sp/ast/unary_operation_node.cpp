#include "unary_operation_node.h"

namespace ast {
UnaryOperationNode::UnaryOperationNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> UnaryOperationNode::GetOperand() { return operand; }

void UnaryOperationNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(
      std::static_pointer_cast<StatementNode>(currentNode), depth);

  operand->AcceptVisitor(operand, extractor, depth + 1);
}
}  // namespace ast
