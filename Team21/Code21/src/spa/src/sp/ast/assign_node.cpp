#include "assign_node.h"

namespace ast {
AssignNode::AssignNode(VariableNode *var, ExpressionNode *exp) {
  this->var = var;
  this->assignment = exp->GetOperand();
}

VariableNode *AssignNode::GetVariable() {
  return var;
}

INode *AssignNode::GetAssignment() {
  return assignment;
}

std::string AssignNode::ToString() const {
  return "assign:\n{\n" + var->ToString() + assignment->ToString() + "}\n";
}
}
