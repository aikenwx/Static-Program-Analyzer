#include "equals_node.h"

namespace ast {
std::string EqualsNode::ToString() const {
  return "equals:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
