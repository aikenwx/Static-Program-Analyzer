#include "ast.h"

#include <utility>

namespace ast {
auto AST::GetRoot() -> std::shared_ptr<INode> { return this->root; }

void AST::SetRoot(std::shared_ptr<INode> node) { root = std::move(node); }
}
