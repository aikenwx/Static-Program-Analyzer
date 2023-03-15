#include "greater_node.h"

namespace ast {
auto GreaterNode::ToString() const -> std::string {
  return "greater:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
