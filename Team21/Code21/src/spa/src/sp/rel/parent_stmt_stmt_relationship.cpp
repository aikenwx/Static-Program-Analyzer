#include <string>

#include "parent_stmt_stmt_relationship.h"
#include "util.h"

namespace rel {
int ParentStmtStmtRelationship::firstStatementNumber() {
  return firstStatementNode_->GetStatementNumber();
};

int ParentStmtStmtRelationship::secondStatementNumber() {
  return secondStatementNode_->GetStatementNumber();
};

EntityType ParentStmtStmtRelationship::firstEntityType() {
  return resolveEntityType(firstStatementNode_);
};

EntityType ParentStmtStmtRelationship::secondEntityType() {
  return resolveEntityType(secondStatementNode_);
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
