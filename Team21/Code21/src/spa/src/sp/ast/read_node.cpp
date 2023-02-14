#include "read_node.h"

namespace ast {
ReadNode::ReadNode(VariableNode *var) {
  this->var = var;
}

VariableNode *ReadNode::GetVariable() {
  return var;
}

std::string ReadNode::ToString() const {
  return "read:\n{\n" + var->ToString() + "}\n";
}
}
