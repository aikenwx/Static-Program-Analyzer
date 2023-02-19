#include "or_node.h"

namespace ast {
std::string OrNode::ToString() const {
  return "or:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
