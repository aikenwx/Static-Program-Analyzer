#include "lesser_node.h"

namespace ast {
std::string LesserNode::ToString() const {
  return "lesser:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
