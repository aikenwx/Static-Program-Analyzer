#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class FollowsStmtStmtRelationship : public StmtStmtRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement) -> std::unique_ptr<FollowsStmtStmtRelationship>;
  auto firstStatementNumber() const -> int override;
  auto secondStatementNumber() const -> int override;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::FOLLOWS_STMT_STMT; };

 private:
  FollowsStmtStmtRelationship(std::shared_ptr<ast::StatementNode> firstStatement,
                              std::shared_ptr<ast::StatementNode> secondStatement);
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};
}  // namespace rel
