#include "equals_node.h"
#include "sp/ast/binary_operation_node.h"

namespace ast {
auto EqualsNode::ToString() const -> std::string {
  return "equals:\n" + BinaryOperationNode::ToString();
}
}
