#include "or_node.h"

namespace ast {
auto OrNode::ToString() const -> std::string {
  return "or:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
