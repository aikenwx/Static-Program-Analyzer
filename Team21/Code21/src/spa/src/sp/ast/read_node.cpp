#include "read_node.h"

namespace ast {
ReadNode::ReadNode(VariableNode *var) {
  this->var = var;
}

VariableNode *ReadNode::getVariable() {
  return var;
}
}
