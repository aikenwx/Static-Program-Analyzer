#include "name_node.h"

namespace ast {
//void NameNode::Execute() {
//  return;
//}

std::ostream &NameNode::Write(std::ostream &out) const {
  out << "Name" << "\n";
  return out;
}
}
