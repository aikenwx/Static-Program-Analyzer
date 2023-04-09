#include "comparison_operation_node.h"

#include <utility>

namespace ast {

ComparisonOperationNode::ComparisonOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, std::string_view type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)), operator_(type) {}

auto ComparisonOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto ComparisonOperationNode::ToString() const -> std::string {
  return "comparison:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
