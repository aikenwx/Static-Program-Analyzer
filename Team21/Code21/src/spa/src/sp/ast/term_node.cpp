#include "term_node.h"

#include <utility>

namespace ast {
TermNode::TermNode(std::shared_ptr<INode> operand)
    : operand(std::move(operand)) {}

auto TermNode::GetOperand() -> std::shared_ptr<INode> { return operand; }

auto TermNode::ToString() const -> std::string {
  return "term:" + operand->ToString() + "\n";
}
}  // namespace ast
