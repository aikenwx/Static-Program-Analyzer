#pragma once

#include "../rel/parent_stmt_stmt_relationship.h"
#include "extractor.h"

namespace design_extractor {
class ParentExtractor : public Extractor {
 public:
  ParentExtractor() = default;
  void HandleIfNode(const std::shared_ptr<ast::IfNode>& node, int depth) override;
  void HandleWhileNode(const std::shared_ptr<ast::WhileNode>& node,
                       int depth) override;

  std::vector<std::shared_ptr<rel::ParentStmtStmtRelationship>>
  GetRelationships();

 private:
  std::vector<std::shared_ptr<rel::ParentStmtStmtRelationship>> relns_;
};
}  // namespace design_extractor
