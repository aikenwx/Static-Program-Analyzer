#include "factor_node.h"

#include <utility>

namespace ast {
FactorNode::FactorNode(std::shared_ptr<INode> operand)
    : operand(std::move(operand)) {}

auto FactorNode::GetOperand() -> std::shared_ptr<INode> { return operand; }

auto FactorNode::ToString() const -> std::string {
  return "factor:" + operand->ToString() + "\n";
}
}  // namespace ast
