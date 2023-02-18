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

std::unique_ptr<ParentStmtStmtRelationship>
ParentStmtStmtRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement) {
  return std::unique_ptr<ParentStmtStmtRelationship>(new ParentStmtStmtRelationship(firstStatement, secondStatement));
};

ParentStmtStmtRelationship::ParentStmtStmtRelationship(
    std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement) {
  firstStatementNode_ = firstStatement;
  secondStatementNode_ = secondStatement;
};
}  // namespace rel
