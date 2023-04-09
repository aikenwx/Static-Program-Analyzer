#include "comparison_operation_node.h"

#include <utility>

namespace ast {

ComparisonOperationNode::ComparisonOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, SymbolType type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)) {
  switch (type) {
  case SymbolType::kEqual:
    operator_ = "==";
    break;
  case SymbolType::kLesser:
    operator_ = "<";
    break;
  case SymbolType::kGreater:
    operator_ = ">";
    break;
  case SymbolType::kNotEqual:
    operator_ = "!=";
    break;
  case SymbolType::kLesserEqual:
    operator_ = "<=";
    break;
  case SymbolType::kGreaterEqual:
    operator_ = ">=";
    break;
  default:
    operator_ = "";
    break;
  }
}

auto ComparisonOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto ComparisonOperationNode::ToString() const -> std::string {
  return "comparison:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
