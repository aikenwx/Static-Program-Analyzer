#include "term_node.h"

namespace ast {
TermNode::TermNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> TermNode::GetOperand() {
  return operand;
}

std::string TermNode::ToString() const {
  return "term:" + operand->ToString() + "\n";
}
}
