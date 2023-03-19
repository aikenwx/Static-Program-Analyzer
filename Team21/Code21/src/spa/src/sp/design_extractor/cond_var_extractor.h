#include "extractor.h"

#include <unordered_map>
#include <unordered_set>

namespace design_extractor {
template <typename T> class CondVarExtractor : public Extractor {
public:
  CondVarExtractor() = default;

  void HandleConditionalNode(
      const std::shared_ptr<ast::ConditionalNode> &node, int depth) override;

  [[nodiscard]] auto stmtNoCondVarMap() const
      -> std::unordered_map<int, std::unordered_set<std::string>> {
    return condVars_;
  };

private:
  std::unordered_map<int, std::unordered_set<std::string>> condVars_;
};
} // namespace design_extractor
