#include "follows_stmt_stmt_relationship.h"

#include <string>
#include <utility>

namespace rel {
auto FollowsStmtStmtRelationship::firstStatementNumber() const -> int {
  return firstStatementNode_->GetStatementNumber();
};

auto FollowsStmtStmtRelationship::secondStatementNumber() const -> int {
  return secondStatementNode_->GetStatementNumber();
};

auto FollowsStmtStmtRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode) -> std::unique_ptr<FollowsStmtStmtRelationship> {
  return std::unique_ptr<FollowsStmtStmtRelationship>(new FollowsStmtStmtRelationship(std::move(firstStatementNode), std::move(secondStatementNode)));
};

FollowsStmtStmtRelationship::FollowsStmtStmtRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode) {
  firstStatementNode_ = firstStatementNode;
  secondStatementNode_ = secondStatementNode;
};
}  // namespace rel
