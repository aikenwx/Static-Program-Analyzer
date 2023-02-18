#include "assign_node.h"

namespace ast {
AssignNode::AssignNode(std::shared_ptr<VariableNode> var, std::shared_ptr<ExpressionNode> exp) {
  this->var = var;
  this->assignment = exp->GetOperand();
}

std::shared_ptr<VariableNode> AssignNode::GetVariable() {
  return var;
}

std::shared_ptr<INode> AssignNode::GetAssignment() {
  return assignment;
}

std::string AssignNode::ToString() const {
  return "assign:\n{\n" + var->ToString() + assignment->ToString() + "}\n";
}
}
