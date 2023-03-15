#include "modulo_node.h"

namespace ast {
auto ModuloNode::ToString() const -> std::string {
  return "modulo:\n{\nleft:" + left->ToString() + "right:" + right->ToString() + "}\n";
}
}
