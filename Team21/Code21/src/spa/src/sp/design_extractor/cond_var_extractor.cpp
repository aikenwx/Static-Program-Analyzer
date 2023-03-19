#include "cond_var_extractor.h"
#include "variable_extractor.h"
#include "sp/ast/conditional_node.h"
#include "util/instance_of.h"
#include <memory>

namespace design_extractor {
template <typename T>
void CondVarExtractor<T>::HandleConditionalNode(
    const std::shared_ptr<ast::ConditionalNode> &node, int depth) {
  if (!util::instance_of<T>(node)) {
    return;
  }

  auto varExtractor = VariableExtractor();
  node->AcceptVisitor(varExtractor, depth);
  condVars_[node->GetStatementNumber()] = varExtractor.vars();
}
} // namespace design_extractor
