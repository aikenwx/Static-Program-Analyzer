#include "identifier_node.h"

namespace ast {
IdentifierNode::IdentifierNode(std::string value) {
  this->value = value;
}

void IdentifierNode::execute() {
  return;
}

std::string IdentifierNode::getValue() {
  return value;
}
}
