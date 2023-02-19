#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class FollowsStmtStmtRelationship : public StmtStmtRelationship {
 public:
  int firstStatementNumber() override;
  int secondStatementNumber() override;
  static std::unique_ptr<FollowsStmtStmtRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement);

 private:
  FollowsStmtStmtRelationship(std::shared_ptr<ast::StatementNode> firstStatement,
                              std::shared_ptr<ast::StatementNode> secondStatement);
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};
}  // namespace rel
