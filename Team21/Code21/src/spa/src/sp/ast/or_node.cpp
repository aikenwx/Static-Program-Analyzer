#include "or_node.h"

namespace ast {
auto OrNode::ToString() const -> std::string {
  return "or:\n" + BinaryOperationNode::ToString();
}
}
