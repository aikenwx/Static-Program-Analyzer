#include "relational_expression_node.h"

#include <utility>

namespace ast {
RelationalExpressionNode::RelationalExpressionNode(
    std::shared_ptr<INode> operand)
    : operand(std::move(std::move(operand))) {}

auto RelationalExpressionNode::GetOperand() -> std::shared_ptr<INode> {
  return operand;
}

auto RelationalExpressionNode::ToString() const -> std::string {
  return "rel_expression:" + operand->ToString() + "\n";
}
}  // namespace ast
