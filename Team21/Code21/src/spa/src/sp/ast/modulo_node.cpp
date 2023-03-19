#include "modulo_node.h"

namespace ast {
auto ModuloNode::ToString() const -> std::string {
  return "modulo:\n" + BinaryOperationNode::ToString();
}
}
