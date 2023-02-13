#include "term_node.h"

namespace ast {
TermNode::TermNode(INode *operand) {
  this->operand = operand;
}

INode *TermNode::GetOperand() {
  return operand;
}

std::ostream &TermNode::Write(std::ostream &out) const {
  return out;
}
}
