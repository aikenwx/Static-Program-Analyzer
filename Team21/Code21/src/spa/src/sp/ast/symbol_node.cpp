#include "symbol_node.h"

namespace ast {
SymbolNode::SymbolNode(SymbolType type) {
  this->type = type;
}

SymbolType SymbolNode::GetType() {
  return type;
}

std::ostream &SymbolNode::Write(std::ostream &out) const {
  out << "Symbol" << "\n";
  return out;
}
}
