#include "relational_factor_node.h"

#include <utility>

namespace ast {
RelationalFactorNode::RelationalFactorNode(std::shared_ptr<INode> operand)
    : operand(std::move(operand)) {}

auto RelationalFactorNode::GetOperand() -> std::shared_ptr<INode> {
  return operand;
}

auto RelationalFactorNode::ToString() const -> std::string {
  return "rel_factor:" + operand->ToString() + "\n";
}
}  // namespace ast
