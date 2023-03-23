#include "multiplicative_operation_node.h"

#include <utility>

namespace ast {

MultiplicativeOperationNode::MultiplicativeOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, SymbolType type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)) {
  switch (type) {
  case SymbolType::kMultiply:
    operator_ = "*";
    break;
  case SymbolType::kDivide:
    operator_ = "/";
    break;
  case SymbolType::kModulo:
    operator_ = "%";
    break;
  default:
    operator_ = "";
    break;
  }
}

auto MultiplicativeOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto MultiplicativeOperationNode::ToString() const -> std::string {
  return "multiplicative:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
