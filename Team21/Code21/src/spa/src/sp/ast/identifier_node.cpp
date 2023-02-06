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

std::ostream &IdentifierNode::Write(std::ostream &out) const {
  out << "Identifier" << "\n";
  return out;
}
}
