#include "symbol_node.h"

namespace ast {
SymbolNode::SymbolNode(std::string_view type) : type_(type) {}

auto SymbolNode::GetType() -> std::string { return type_; }

auto SymbolNode::ToString() const -> std::string {
  return "symbol:\n";
}
}  // namespace ast
