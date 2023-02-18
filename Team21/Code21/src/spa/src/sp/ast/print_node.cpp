#include "print_node.h"

namespace ast {
PrintNode::PrintNode(VariableNode *var) {
  this->var = var;
}

VariableNode *PrintNode::GetVariable() {
  return var;
}

std::ostream &PrintNode::Write(std::ostream &out) const {
  out << "print:" << "\n{";
  var->Write(out);
  out << "}" << "\n";
  return out;
}
}
