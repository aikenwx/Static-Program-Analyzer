#include "greater_equals_node.h"

namespace ast {
auto GreaterEqualsNode::ToString() const -> std::string {
  return "greater_equals:\n" + BinaryOperationNode::ToString();
}
}
