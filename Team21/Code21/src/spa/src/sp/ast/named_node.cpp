#include "named_node.h"

namespace ast {
NamedNode::NamedNode(std::string name) {
  this->name = name;
}

//void NamedNode::Execute() {
//  return;
//}

std::string NamedNode::GetName() {
  return name;
}
}
