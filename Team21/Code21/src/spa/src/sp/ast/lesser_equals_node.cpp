#include "lesser_equals_node.h"

namespace ast {
std::string LesserEqualsNode::ToString() const {
  return "lesser_equals:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
