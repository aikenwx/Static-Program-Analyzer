#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class FollowsStmtStmtRelationship : public StmtStmtRelationship {
 public:
  static std::unique_ptr<FollowsStmtStmtRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement);
  int firstStatementNumber() override;
  int secondStatementNumber() override;
  RelationshipType relationshipType() override { return RelationshipType::FOLLOWS_STMT_STMT; };

 private:
  FollowsStmtStmtRelationship(std::shared_ptr<ast::StatementNode> firstStatement,
                              std::shared_ptr<ast::StatementNode> secondStatement);
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};
}  // namespace rel
