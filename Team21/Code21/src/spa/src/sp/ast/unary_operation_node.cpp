#include "unary_operation_node.h"

#include <utility>

namespace ast {
UnaryOperationNode::UnaryOperationNode(std::shared_ptr<INode> operand)
    : operand(std::move(std::move(operand))) {}

auto UnaryOperationNode::GetOperand() -> std::shared_ptr<INode> {
  return operand;
}

void UnaryOperationNode::AcceptVisitor(
    const std::shared_ptr<INode>& currentNode,
    const std::shared_ptr<design_extractor::Extractor>& extractor, int depth) {
  // nb: extractor doesn't have a HandleUnaryOperationNode method
  // call HandleOtherNode() to let extractor update state
  // e.g. node parents
  extractor->HandleOtherNode(currentNode, depth);

  operand->AcceptVisitor(operand, extractor, depth + 1);
}
}  // namespace ast
