#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ParentStmtStmtRelationship : public StmtStmtRelationship {
 public:
  int firstStatementNumber() override;
  int secondStatementNumber() override;
  int firstEntityType() override;
  int secondEntityType() override;
  static ParentStmtStmtRelationship CreateRelationship(
      ast::StatementNode* firstStatement, ast::StatementNode* secondStatement);

 private:
  ParentStmtStmtRelationship(
      ast::StatementNode* firstStatement, ast::StatementNode* secondStatement
  );
  ast::StatementNode* firstStatementNode_;
  ast::StatementNode* secondStatementNode_;
};
}  // namespace rel
