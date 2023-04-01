#include "identifier_node.h"

#include <utility>

namespace ast {
IdentifierNode::IdentifierNode(std::string value)
    : value(std::move(value)) {}

auto IdentifierNode::GetValue() -> std::string { return value; }

auto IdentifierNode::ToString() const -> std::string {
  return "Identifier:" + value + "\n";
}

void IdentifierNode::AcceptVisitor(design_extractor::Extractor &extractor,
                                 int depth) {
  extractor.HandleVariableNode(
      std::static_pointer_cast<IdentifierNode>(shared_from_this()), depth);
}
}  // namespace ast
