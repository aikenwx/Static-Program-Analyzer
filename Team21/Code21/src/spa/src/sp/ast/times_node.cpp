#include "times_node.h"

namespace ast {
auto TimesNode::ToString() const -> std::string {
  return "times:\n" + BinaryOperationNode::ToString();
}
}
