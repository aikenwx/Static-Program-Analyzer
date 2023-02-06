#include "symbol_node.h"

namespace ast {
SymbolNode::SymbolNode(SymbolType type) {
  this->type = type;
}

SymbolType SymbolNode::GetType() {
  return type;
}
}
