#include "greater_equals_node.h"

namespace ast {
auto GreaterEqualsNode::ToString() const -> std::string {
  return "greater_equals:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
