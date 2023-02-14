#include "print_node.h"

namespace ast {
PrintNode::PrintNode(VariableNode *var) {
  this->var = var;
}

VariableNode *PrintNode::GetVariable() {
  return var;
}

std::string PrintNode::ToString() const {
  return "print:\n{\n" + var->ToString() + "}\n";
}
}
