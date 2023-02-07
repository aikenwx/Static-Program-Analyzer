#include "stmt_relationship.h"

namespace rel {
int PrintStmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

EntityType PrintStmtRelationship::entityType() {
  return EntityType::PRINT_STATEMENT;
};

PrintStmtRelationship* PrintStmtRelationship::CreateRelationship(ast::PrintNode* statementNode) {
  return new PrintStmtRelationship(statementNode);
};

PrintStmtRelationship::PrintStmtRelationship(ast::PrintNode* statementNode) {
  statementNode_ = statementNode;
};

int ReadStmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

EntityType ReadStmtRelationship::entityType() {
  return EntityType::READ_STATEMENT;
};

ReadStmtRelationship* ReadStmtRelationship::CreateRelationship(ast::ReadNode* statementNode) {
  return new ReadStmtRelationship(statementNode);
};

ReadStmtRelationship::ReadStmtRelationship(ast::ReadNode* statementNode) {
  statementNode_ = statementNode;
};
}
