#include "identifier_node.h"

namespace ast {
IdentifierNode::IdentifierNode(std::string value) {
  this->value = value;
}

void IdentifierNode::Execute() {
  return;
}

std::string IdentifierNode::GetValue() {
  return value;
}

std::string IdentifierNode::ToString() const {
  return "Identifier:" + value + "\n";
}
}
