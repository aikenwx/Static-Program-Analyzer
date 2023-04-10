#pragma once

#include <string>
#include <utility>

#include "relationship.h"

namespace rel {
class FollowsStmtStmtRelationship : public StmtStmtRelationship {
public:
  static auto
  CreateRelationship(std::shared_ptr<ast::StatementNode> firstStatement,
                     std::shared_ptr<ast::StatementNode> secondStatement)
      -> std::unique_ptr<FollowsStmtStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override {
    return RelationshipType::FOLLOWS_STMT_STMT;
  };

private:
  FollowsStmtStmtRelationship(
      std::shared_ptr<ast::StatementNode> firstStatementNode,
      std::shared_ptr<ast::StatementNode> secondStatementNode)
      : StmtStmtRelationship(std::move(firstStatementNode),
                             std::move(secondStatementNode)){};
};
} // namespace rel
