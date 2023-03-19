#include "extractor.h"
#include "sp/ast/conditional_node.h"
#include "util/instance_of.h"
#include "variable_extractor.h"

#include <unordered_map>
#include <unordered_set>

namespace design_extractor {
template <typename T> class CondVarExtractor : public Extractor {
public:
  CondVarExtractor() = default;

  void HandleConditionalNode(const std::shared_ptr<ast::ConditionalNode> &node,
                             int depth) override {
    if (!util::instance_of<T>(node)) {
      return;
    }

    auto varExtractor = VariableExtractor();
    node->AcceptVisitor(varExtractor, depth);
    condVars_[node->GetStatementNumber()] = varExtractor.vars();
  }

  [[nodiscard]] auto condVars() const
      -> std::unordered_map<int, std::unordered_set<std::string>> {
    return condVars_;
  };

private:
  std::unordered_map<int, std::unordered_set<std::string>> condVars_;
};
} // namespace design_extractor
