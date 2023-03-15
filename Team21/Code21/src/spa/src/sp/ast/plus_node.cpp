#include "plus_node.h"

namespace ast {
auto PlusNode::ToString() const -> std::string {
  return "plus:\n" + BinaryOperationNode::ToString();
}
}
