#include "divide_node.h"

namespace ast {
std::string DivideNode::ToString() const {
  return "divide:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
