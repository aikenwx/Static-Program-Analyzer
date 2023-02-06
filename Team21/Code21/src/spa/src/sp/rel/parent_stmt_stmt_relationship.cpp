#include <string>

#include "parent_stmt_stmt_relationship.h"

namespace rel {
int ParentStmtStmtRelationship::firstStatementNumber() {
  return firstStatementNode_->GetStatementNumber();
};

int ParentStmtStmtRelationship::secondStatementNumber() {
  return secondStatementNode_->GetStatementNumber();
};

int ParentStmtStmtRelationship::firstEntityType() {
  return 0; // TODO
};

int ParentStmtStmtRelationship::secondEntityType() {
  return 0; // TODO
};

ParentStmtStmtRelationship
ParentStmtStmtRelationship::CreateRelationship(
    ast::StatementNode* firstStatement, ast::StatementNode* secondStatement) {
  return ParentStmtStmtRelationship(firstStatement, secondStatement);
};

ParentStmtStmtRelationship::ParentStmtStmtRelationship(
    ast::StatementNode* firstStatement, ast::StatementNode* secondStatement) {
  firstStatementNode_ = firstStatement;
  secondStatementNode_ = secondStatement;
};
}  // namespace rel
