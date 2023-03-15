#include "named_node.h"

#include <utility>

namespace ast {
NamedNode::NamedNode(std::string name) : name(std::move(name)) {}

//void NamedNode::Execute() {
//  return;
//}

auto NamedNode::GetName() -> std::string { return name; }
}  // namespace ast
