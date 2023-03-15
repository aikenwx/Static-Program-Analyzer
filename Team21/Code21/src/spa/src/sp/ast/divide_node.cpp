#include "divide_node.h"
#include "sp/ast/binary_operation_node.h"

namespace ast {
auto DivideNode::ToString() const -> std::string {
  return "divide:\n" + BinaryOperationNode::ToString();
}
}
