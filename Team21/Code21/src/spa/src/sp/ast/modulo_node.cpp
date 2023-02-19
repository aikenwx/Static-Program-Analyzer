#include "modulo_node.h"

namespace ast {
std::string ModuloNode::ToString() const {
  return "modulo:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
