#include "named_node.h"

namespace ast {
NamedNode::NamedNode(std::string name) {
  this->name = name;
}

//void NamedNode::execute() {
//  return;
//}

std::string NamedNode::getName() {
  return name;
}
}
