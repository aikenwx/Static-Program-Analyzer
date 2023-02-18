#include "read_node.h"

namespace ast {
ReadNode::ReadNode(VariableNode *var) {
  this->var = var;
}

VariableNode *ReadNode::GetVariable() {
  return var;
}

std::ostream &ReadNode::Write(std::ostream &out) const {
  out << "read:" << "\n{";
  var->Write(out);
  out << "}" << "\n";
  return out;
}
}
