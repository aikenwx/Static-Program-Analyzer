#include "follows_stmt_stmt_relationship.h"

#include <string>

#include "util.h"

namespace rel {
int FollowsStmtStmtRelationship::firstStatementNumber() {
  return firstStatementNode_->GetStatementNumber();
};

int FollowsStmtStmtRelationship::secondStatementNumber() {
  return secondStatementNode_->GetStatementNumber();
};

EntityType FollowsStmtStmtRelationship::firstEntityType() {
  return resolveEntityType(secondStatementNode_);
};

EntityType FollowsStmtStmtRelationship::secondEntityType() {
  return resolveEntityType(secondStatementNode_);
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
