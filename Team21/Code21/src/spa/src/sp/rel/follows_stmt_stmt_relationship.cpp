#include "follows_stmt_stmt_relationship.h"

#include <string>
#include <utility>

namespace rel {
int FollowsStmtStmtRelationship::firstStatementNumber() const {
  return firstStatementNode_->GetStatementNumber();
};

int FollowsStmtStmtRelationship::secondStatementNumber() const {
  return secondStatementNode_->GetStatementNumber();
};

std::unique_ptr<FollowsStmtStmtRelationship> FollowsStmtStmtRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode) {
  return std::unique_ptr<FollowsStmtStmtRelationship>(new FollowsStmtStmtRelationship(std::move(firstStatementNode), std::move(secondStatementNode)));
};

FollowsStmtStmtRelationship::FollowsStmtStmtRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode) {
  firstStatementNode_ = firstStatementNode;
  secondStatementNode_ = secondStatementNode;
};
}  // namespace rel
