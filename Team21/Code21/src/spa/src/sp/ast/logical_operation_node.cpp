#include "logical_operation_node.h"
#include "sp/ast/symbol_node.h"

#include <utility>

namespace ast {

LogicalOperationNode::LogicalOperationNode(
    std::shared_ptr<INode> left, std::shared_ptr<INode> right, SymbolType type)
    : BinaryOperationNode::BinaryOperationNode(std::move(left), std::move(right)) {
  switch (type) {
  case SymbolType::kAnd:
    operator_ = "&&";
    break;
  case SymbolType::kOr:
    operator_ = "||";
    break;
  default:
    operator_ = "";
    break;
  }
}

auto LogicalOperationNode::GetSymbolType() const -> std::string {
  return operator_;
}

auto LogicalOperationNode::ToString() const -> std::string {
  return "logical:\n" + BinaryOperationNode::ToString();
}
} // namespace ast
