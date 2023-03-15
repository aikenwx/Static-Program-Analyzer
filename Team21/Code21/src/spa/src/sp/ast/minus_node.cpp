#include "minus_node.h"

namespace ast {
auto MinusNode::ToString() const -> std::string {
  return "minus:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
