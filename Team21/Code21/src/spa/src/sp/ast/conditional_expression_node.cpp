#include "conditional_expression_node.h"

#include <utility>

namespace ast {
ConditionalExpressionNode::ConditionalExpressionNode(
    std::shared_ptr<INode> operand)
    : operand(std::move(operand)) {}

auto ConditionalExpressionNode::GetOperand() -> std::shared_ptr<INode> {
  return operand;
}

auto ConditionalExpressionNode::ToString() const -> std::string {
  return "cond_expression:" + operand->ToString() + "\n";
}
}  // namespace ast
