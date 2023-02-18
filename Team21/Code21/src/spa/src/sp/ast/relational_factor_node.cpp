#include "relational_factor_node.h"

namespace ast {
RelationalFactorNode::RelationalFactorNode(INode *operand) {
  this->operand = operand;
}

INode *RelationalFactorNode::GetOperand() {
  return operand;
}

std::string RelationalFactorNode::ToString() const {
  return "rel_factor:" + operand->ToString() + "\n";
}
}
