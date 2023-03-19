#include "variable_extractor.h"
#include "sp/ast/variable_node.h"

namespace design_extractor {
  void VariableExtractor::HandleVariableNode(const std::shared_ptr<ast::VariableNode> &node, int depth) {
    vars_.insert(node->GetName());
  }
}
