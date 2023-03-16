#include "minus_node.h"

namespace ast {
auto MinusNode::ToString() const -> std::string {
  return "minus:\n" + BinaryOperationNode::ToString();
}
}
