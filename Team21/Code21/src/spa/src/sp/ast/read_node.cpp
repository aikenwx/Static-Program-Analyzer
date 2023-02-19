#include "read_node.h"

namespace ast {
ReadNode::ReadNode(std::shared_ptr<VariableNode> var) {
  this->var = var;
}

std::shared_ptr<VariableNode> ReadNode::GetVariable() {
  return var;
}

std::string ReadNode::ToString() const {
  return "read:\n{\n" + var->ToString() + "}\n";
}
}
