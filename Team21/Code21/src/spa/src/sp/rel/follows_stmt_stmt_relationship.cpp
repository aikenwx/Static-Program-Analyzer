#include "follows_stmt_stmt_relationship.h"

#include <string>

namespace rel {
int FollowsStmtStmtRelationship::firstStatementNumber() {
  return firstStatementNode_->GetStatementNumber();
};

int FollowsStmtStmtRelationship::secondStatementNumber() {
  return secondStatementNode_->GetStatementNumber();
};

std::unique_ptr<FollowsStmtStmtRelationship> FollowsStmtStmtRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode) {
  return std::unique_ptr<FollowsStmtStmtRelationship>(new FollowsStmtStmtRelationship(firstStatementNode, secondStatementNode));
};

FollowsStmtStmtRelationship::FollowsStmtStmtRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode) {
  firstStatementNode_ = firstStatementNode;
  secondStatementNode_ = secondStatementNode;
};
}  // namespace rel
