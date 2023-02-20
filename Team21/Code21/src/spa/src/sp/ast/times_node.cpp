#include "times_node.h"

namespace ast {
std::string TimesNode::ToString() const {
  return "times:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
