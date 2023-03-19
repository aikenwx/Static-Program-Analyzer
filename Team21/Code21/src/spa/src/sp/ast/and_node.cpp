#include "and_node.h"
#include "sp/ast/binary_operation_node.h"

namespace ast {
auto AndNode::ToString() const -> std::string {
  return "and:\n" + BinaryOperationNode::ToString();
}
}
