#include "variable_node.h"

namespace ast {
//void VariableNode::execute() {
//  return;
//}

std::ostream &VariableNode::Write(std::ostream &out) const {
  out << "var:" << name << "\n";
  return out;
}
}
