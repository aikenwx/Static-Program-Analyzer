#include "and_node.h"

namespace ast {
std::string AndNode::ToString() const {
  return "and:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
