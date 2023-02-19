#include "greater_node.h"

namespace ast {
std::string GreaterNode::ToString() const {
  return "greater:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
