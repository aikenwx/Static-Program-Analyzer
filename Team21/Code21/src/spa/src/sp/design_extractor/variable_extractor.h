#include "extractor.h"

#include <string>
#include <unordered_set>

namespace design_extractor {
class VariableExtractor : public Extractor {
public:
  void HandleVariableNode(const std::shared_ptr<ast::VariableNode> &node,
                          int depth) override;

  [[nodiscard]] auto vars() const -> std::unordered_set<std::string> {
    return vars_;
  };

private:
  std::unordered_set<std::string> vars_;
};
} // namespace design_extractor
