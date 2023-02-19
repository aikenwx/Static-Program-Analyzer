#include "relational_factor_node.h"

namespace ast {
RelationalFactorNode::RelationalFactorNode(std::shared_ptr<INode> operand) {
  this->operand = operand;
}

std::shared_ptr<INode> RelationalFactorNode::GetOperand() {
  return operand;
}

std::string RelationalFactorNode::ToString() const {
  return "rel_factor:" + operand->ToString() + "\n";
}
}
