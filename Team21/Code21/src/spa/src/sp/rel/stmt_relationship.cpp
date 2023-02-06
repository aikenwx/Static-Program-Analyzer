#include "stmt_relationship.h"

namespace rel {
int PrintStmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

int PrintStmtRelationship::entityType() {
  return 0; // TODO
};

PrintStmtRelationship PrintStmtRelationship::CreateRelationship(ast::PrintNode* statementNode) {
  return PrintStmtRelationship(statementNode);
};

PrintStmtRelationship::PrintStmtRelationship(ast::PrintNode* statementNode) {
  statementNode_ = statementNode;
};

int ReadStmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

int ReadStmtRelationship::entityType() {
  return 0; // TODO
};

ReadStmtRelationship ReadStmtRelationship::CreateRelationship(ast::ReadNode* statementNode) {
  return ReadStmtRelationship(statementNode);
};

ReadStmtRelationship::ReadStmtRelationship(ast::ReadNode* statementNode) {
  statementNode_ = statementNode;
};
}
