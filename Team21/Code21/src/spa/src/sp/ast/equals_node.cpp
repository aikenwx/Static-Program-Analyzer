#include "equals_node.h"

namespace ast {
auto EqualsNode::ToString() const -> std::string {
  return "equals:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
