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

FollowsStmtStmtRelationship* FollowsStmtStmtRelationship::CreateRelationship(
    ast::StatementNode* firstStatementNode,
    ast::StatementNode* secondStatementNode) {
  return new FollowsStmtStmtRelationship(firstStatementNode, secondStatementNode);
};

FollowsStmtStmtRelationship::FollowsStmtStmtRelationship(
    ast::StatementNode* firstStatementNode,
    ast::StatementNode* secondStatementNode) {
  firstStatementNode_ = firstStatementNode;
  secondStatementNode_ = secondStatementNode;
};
}  // namespace rel
