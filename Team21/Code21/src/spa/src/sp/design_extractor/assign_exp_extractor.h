#pragma once

#include "../rel/assign_exp_relationship.h"
#include "extractor.h"

namespace design_extractor {
class AssignExpExtractor : public Extractor {
  // nb: will only handle statement-uses relns, which means that e.g.:
  // - Uses(call, v) will not be included here, but should be derived in SP
  //   facade based on Uses(proc, v)
  // - Uses(proc, v) will not be included here, but should be derived in SP
  //   facade based on proc stmt numbers and Uses(stmt, v)
  // - Uses(container, v) will not be included here, but should be derived
  //   in SP facade based on Parent()
  // - Uses(print, v) will always hold
  // - Uses(assg, v) will always hold (if v is on RHS of assg)
 public:
  AssignExpExtractor() = default;

  void HandleAssignNode(const std::shared_ptr<ast::AssignNode>& node,
                        int depth) override;

  [[nodiscard]] auto GetRelationships() const -> std::vector<std::shared_ptr<rel::AssignExpRelationship>>;
 private:
  std::vector<std::shared_ptr<rel::AssignExpRelationship>> relns_;
};
}  // namespace design_extractor
