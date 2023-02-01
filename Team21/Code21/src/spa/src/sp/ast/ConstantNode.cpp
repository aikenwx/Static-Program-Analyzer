#include "ConstantNode.h"

ConstantNode::ConstantNode(int value) {
  this->value = value;
}

void ConstantNode::execute() {

}

int ConstantNode::getValue() {
  return this->value;
}