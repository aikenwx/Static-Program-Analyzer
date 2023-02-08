#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class FollowsStmtStmtRelationship : public StmtStmtRelationship {
 public:
  int firstStatementNumber() override;
  int secondStatementNumber() override;
  EntityType firstEntityType() override;
  EntityType secondEntityType() override;
  static FollowsStmtStmtRelationship* CreateRelationship(
      ast::StatementNode* firstStatement, ast::StatementNode* secondStatement);

 private:
  FollowsStmtStmtRelationship(ast::StatementNode* firstStatement,
                              ast::StatementNode* secondStatement);
  ast::StatementNode* firstStatementNode_;
  ast::StatementNode* secondStatementNode_;
};
}  // namespace rel
