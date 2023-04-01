#include "sp/ast/identifier_node.h"
#include "variable_extractor.h"

namespace design_extractor {
  void VariableExtractor::HandleVariableNode(const std::shared_ptr<ast::IdentifierNode> &node, int depth) {
    vars_.insert(node->GetValue());
  }
}
