#include <string>

#include "parent_stmt_stmt_relationship.h"

namespace rel {
int ParentStmtStmtRelationship::firstStatementNumber() const {
  return firstStatementNode_->GetStatementNumber();
};

int ParentStmtStmtRelationship::secondStatementNumber() const {
  return secondStatementNode_->GetStatementNumber();
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
