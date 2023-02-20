#include "constant_node.h"

namespace ast {
ConstantNode::ConstantNode(int value) {
  this->value = value;
}

int ConstantNode::GetValue() {
  return this->value;
}

std::string ConstantNode::ToString() const {
  return "constant:" + std::to_string(value) + "\n";
}
}
