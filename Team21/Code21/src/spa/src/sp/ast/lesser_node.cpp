#include "lesser_node.h"

namespace ast {
auto LesserNode::ToString() const -> std::string {
  return "lesser:\n" + BinaryOperationNode::ToString();
}
}
