#include "multiplicative_operation_node.h"

#include <utility>

namespace ast {

MultiplicativeOperationNode::MultiplicativeOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, std::string_view type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)), operator_(type) {}

auto MultiplicativeOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto MultiplicativeOperationNode::ToString() const -> std::string {
  return "multiplicative:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
