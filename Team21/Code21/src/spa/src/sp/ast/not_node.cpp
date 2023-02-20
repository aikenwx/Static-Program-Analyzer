#include "not_node.h"

namespace ast {
std::string NotNode::ToString() const {
  return "not:\n{\n" + operand->ToString() + "}\n";
}
}
