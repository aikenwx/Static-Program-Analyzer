#include "unary_operation_node.h"

#include <utility>

namespace ast {
UnaryOperationNode::UnaryOperationNode(std::shared_ptr<INode> operand)
    : operand(std::move(operand)) {}

auto UnaryOperationNode::GetOperand() const -> std::shared_ptr<INode> {
  return operand;
}

auto UnaryOperationNode::ToString() const -> std::string {
  return "{\n" + operand->ToString() + "}\n";
}

void UnaryOperationNode::AcceptVisitor(design_extractor::Extractor &extractor,
                                       int depth) {
  // nb: extractor doesn't have a HandleUnaryOperationNode method
  // call HandleOtherNode() to let extractor update state
  // e.g. node parents
  extractor.HandleOtherNode(shared_from_this(), depth);

  operand->AcceptVisitor(extractor, depth + 1);
}
} // namespace ast
