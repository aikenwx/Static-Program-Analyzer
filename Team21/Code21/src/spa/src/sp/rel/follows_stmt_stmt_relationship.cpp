#include <string>

#include "follows_stmt_stmt_relationship.h"

namespace rel {
int FollowsStmtStmtRelationship::firstStatementNumber() {
  return firstStatementNode_->GetStatementNumber();
};

int FollowsStmtStmtRelationship::secondStatementNumber() {
  return secondStatementNode_->GetStatementNumber();
};

int FollowsStmtStmtRelationship::firstEntityType() {
  return 0; // TODO
};

int FollowsStmtStmtRelationship::secondEntityType() {
  return 0; // TODO
};

FollowsStmtStmtRelationship
FollowsStmtStmtRelationship::CreateRelationship(
    ast::StatementNode* firstStatementNode,
    ast::StatementNode* secondStatementNode) {
  return FollowsStmtStmtRelationship(firstStatementNode,
                                         secondStatementNode);
};

FollowsStmtStmtRelationship::FollowsStmtStmtRelationship(
    ast::StatementNode* firstStatementNode,
    ast::StatementNode* secondStatementNode) {
  firstStatementNode_ = firstStatementNode;
  secondStatementNode_ = secondStatementNode;
};
}  // namespace rel
