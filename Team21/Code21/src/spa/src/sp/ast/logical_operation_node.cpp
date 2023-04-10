#include "logical_operation_node.h"
#include "sp/ast/symbol_node.h"

#include <utility>

namespace ast {

LogicalOperationNode::LogicalOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, std::string_view type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)), operator_(type) {}

auto LogicalOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto LogicalOperationNode::ToString() const -> std::string {
  return "logical:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
