#include "constant_node.h"

namespace ast {
ConstantNode::ConstantNode(int value) {
  this->value = value;
}

void ConstantNode::Execute() {

}

int ConstantNode::GetValue() {
  return this->value;
}

std::ostream &ConstantNode::Write(std::ostream &out) const {
  out << "constant:" << value << "\n";
  return out;
}
}
