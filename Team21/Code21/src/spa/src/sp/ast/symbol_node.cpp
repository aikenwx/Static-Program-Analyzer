#include "symbol_node.h"

namespace ast {
SymbolNode::SymbolNode(SymbolType type) : type(type) {}

auto SymbolNode::GetType() -> SymbolType { return type; }

auto SymbolNode::ToString() const -> std::string {
  // TODO(zhongfu): differentiate symbols
  return "symbol:\n";
}
}  // namespace ast
