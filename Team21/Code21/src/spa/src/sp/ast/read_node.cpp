#include "read_node.h"

ReadNode::ReadNode(VariableNode *var) {
  this->var = var;
}

VariableNode *ReadNode::getVariable() {
  return var;
}