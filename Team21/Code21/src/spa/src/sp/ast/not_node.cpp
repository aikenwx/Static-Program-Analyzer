#include "not_node.h"

namespace ast {
auto NotNode::ToString() const -> std::string {
  return "not:\n{\n" + operand->ToString() + "}\n";
}
}
