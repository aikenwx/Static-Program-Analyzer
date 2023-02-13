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

std::ostream &AssignNode::Write(std::ostream &out) const {
  out << "assign:" << "\n{";
  var->Write(out);
  out << "}" << "\n";
  return out;
}
}
