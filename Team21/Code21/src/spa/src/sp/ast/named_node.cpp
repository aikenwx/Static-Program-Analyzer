#include "named_node.h"

#include <utility>

namespace ast {
NamedNode::NamedNode(std::string name) : name(std::move(name)) {}

auto NamedNode::GetName() const -> std::string { return name; }

auto NamedNode::ToString() const -> std::string { return name + "\n"; }
}  // namespace ast
