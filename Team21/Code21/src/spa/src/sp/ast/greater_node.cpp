#include "greater_node.h"

namespace ast {
auto GreaterNode::ToString() const -> std::string {
  return "greater:\n" + BinaryOperationNode::ToString();
}
}
