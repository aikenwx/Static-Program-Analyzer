#include "lesser_equals_node.h"

namespace ast {
auto LesserEqualsNode::ToString() const -> std::string {
  return "lesser_equals:\n" + BinaryOperationNode::ToString();
}
}
