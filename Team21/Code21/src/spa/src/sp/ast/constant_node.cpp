#include "constant_node.h"

namespace ast {
ConstantNode::ConstantNode(int value) {
  this->value = value;
}

void ConstantNode::execute() {

}

int ConstantNode::getValue() {
  return this->value;
}
}
