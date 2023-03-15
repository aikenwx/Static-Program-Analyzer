#include "plus_node.h"

namespace ast {
auto PlusNode::ToString() const -> std::string {
  return "plus:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
