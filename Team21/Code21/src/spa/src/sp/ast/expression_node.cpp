#include "expression_node.h"

#include <utility>

namespace ast {
ExpressionNode::ExpressionNode(std::shared_ptr<INode> operand)
    : operand(std::move(std::move(operand))) {}

auto ExpressionNode::GetOperand() -> std::shared_ptr<INode> { return operand; }

auto ExpressionNode::ToString() const -> std::string {
  return "expression:" + operand->ToString() + "\n";
}
}  // namespace ast
