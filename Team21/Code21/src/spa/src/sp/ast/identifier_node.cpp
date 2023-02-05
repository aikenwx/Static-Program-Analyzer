#include "identifier_node.h"

IdentifierNode::IdentifierNode(std::string value) {
  this->value = value;
}

void IdentifierNode::execute() {
  return;
}

std::string IdentifierNode::getValue() {
  return value;
}