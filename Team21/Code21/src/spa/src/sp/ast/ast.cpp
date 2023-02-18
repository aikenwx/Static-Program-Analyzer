#include "ast.h"

namespace ast {
std::shared_ptr<INode> AST::GetRoot() {
  return this->root;
}

void AST::SetRoot(std::shared_ptr<INode> node) {
  root = node;
}
}
