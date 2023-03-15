#include "and_node.h"

namespace ast {
auto AndNode::ToString() const -> std::string {
  return "and:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
