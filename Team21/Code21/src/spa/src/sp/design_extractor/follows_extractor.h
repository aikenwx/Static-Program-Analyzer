#pragma once

#include <optional>

#include "../rel/follows_stmt_stmt_relationship.h"
#include "extractor.h"

namespace design_extractor {
class FollowsExtractor : public Extractor {
public:
  FollowsExtractor() = default;
  void
  HandleStatementListNode(const std::shared_ptr<ast::StatementListNode> &node,
                          int depth) override;

  [[nodiscard]] auto GetRelationships() const
      -> std::vector<std::shared_ptr<rel::FollowsStmtStmtRelationship>>;

private:
  std::vector<std::shared_ptr<rel::FollowsStmtStmtRelationship>> relns_;
};
} // namespace design_extractor
