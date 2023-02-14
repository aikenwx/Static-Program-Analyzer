#include "term_node.h"

namespace ast {
TermNode::TermNode(INode *operand) {
  this->operand = operand;
}

INode *TermNode::GetOperand() {
  return operand;
}

std::string TermNode::ToString() const {
  return "term:" + operand->ToString() + "\n";
}
}
