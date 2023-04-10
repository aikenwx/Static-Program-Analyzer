#include "additive_operation_node.h"

#include <utility>

namespace ast {

AdditiveOperationNode::AdditiveOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, std::string_view type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)), operator_(type) {}

auto AdditiveOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto AdditiveOperationNode::ToString() const -> std::string {
  return "additive:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
