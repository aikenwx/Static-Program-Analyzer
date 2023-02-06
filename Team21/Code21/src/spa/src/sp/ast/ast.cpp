#include "ast.h"

namespace ast {
INode *AST::GetRoot() {
  return this->root;
}

void AST::SetRoot(INode *node) {
  root = node;
}
}
