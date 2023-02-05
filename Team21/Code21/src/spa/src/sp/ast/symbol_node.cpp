#include "symbol_node.h"

SymbolNode::SymbolNode(SymbolType type) {
  this->type = type;
}

SymbolType SymbolNode::getType() {
  return type;
}