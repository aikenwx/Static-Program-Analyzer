#include "additive_operation_node.h"

#include <utility>

namespace ast {

AdditiveOperationNode::AdditiveOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, SymbolType type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)) {
  switch (type) {
  case SymbolType::kPlus:
    operator_ = "+";
    break;
  case SymbolType::kMinus:
    operator_ = "-";
    break;
  default:
    operator_ = "";
    break;
  }
}

auto AdditiveOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto AdditiveOperationNode::ToString() const -> std::string {
  return "additive:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
