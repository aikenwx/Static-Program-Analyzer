#include "divide_node.h"

namespace ast {
auto DivideNode::ToString() const -> std::string {
  return "divide:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
