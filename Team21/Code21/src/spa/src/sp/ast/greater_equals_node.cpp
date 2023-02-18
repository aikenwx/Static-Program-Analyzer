#include "greater_equals_node.h"

namespace ast {
std::string GreaterEqualsNode::ToString() const {
  return "greater_equals:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
