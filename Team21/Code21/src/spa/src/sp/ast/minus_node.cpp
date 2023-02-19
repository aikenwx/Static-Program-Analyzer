#include "minus_node.h"

namespace ast {
std::string MinusNode::ToString() const {
  return "minus:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
