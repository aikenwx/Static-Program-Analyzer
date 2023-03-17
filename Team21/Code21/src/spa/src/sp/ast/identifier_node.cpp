#include "identifier_node.h"

#include <utility>

namespace ast {
IdentifierNode::IdentifierNode(std::string value)
    : value(std::move(value)) {}

void IdentifierNode::Execute() {}

auto IdentifierNode::GetValue() -> std::string { return value; }

auto IdentifierNode::ToString() const -> std::string {
  return "Identifier:" + value + "\n";
}
}  // namespace ast
