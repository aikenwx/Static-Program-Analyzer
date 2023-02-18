#include "not_node.h"

namespace ast {
NotNode::NotNode(INode *operand) {
  this->operand = operand;
}

INode *NotNode::GetOperand() {
  return operand;
}

std::string NotNode::ToString() const {
  return "not:\n{\n" + operand->ToString() + "}\n";
}
}