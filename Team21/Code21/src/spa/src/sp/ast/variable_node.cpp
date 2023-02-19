#include "variable_node.h"

namespace ast {
std::string VariableNode::ToString() const {
  return "variable:" + name + "\n";
}
}
