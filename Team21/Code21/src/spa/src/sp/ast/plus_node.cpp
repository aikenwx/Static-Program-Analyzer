#include "plus_node.h"

namespace ast {
std::string PlusNode::ToString() const {
  return "plus:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
