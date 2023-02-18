#include "not_equals_node.h"

namespace ast {
std::string NotEqualsNode::ToString() const {
  return "not_equals:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
