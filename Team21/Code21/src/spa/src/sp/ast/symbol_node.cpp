#include "symbol_node.h"

namespace ast {
SymbolNode::SymbolNode(SymbolType type) {
  this->type = type;
}

SymbolType SymbolNode::GetType() {
  return type;
}

std::string SymbolNode::ToString() const {
  // TODO differentiate symbols
  return "symbol:\n";
}
}
