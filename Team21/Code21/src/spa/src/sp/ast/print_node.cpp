#include "print_node.h"

namespace ast {
PrintNode::PrintNode(std::shared_ptr<VariableNode> var) {
  this->var = var;
}

std::shared_ptr<VariableNode> PrintNode::GetVariable() {
  return var;
}

std::string PrintNode::ToString() const {
  return "print:\n{\n" + var->ToString() + "}\n";
}
}
