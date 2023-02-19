#include "stmt_relationship.h"

namespace rel {
int PrintStmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

EntityType PrintStmtRelationship::entityType() {
  return EntityType::PRINT_STATEMENT;
};

std::unique_ptr<PrintStmtRelationship> PrintStmtRelationship::CreateRelationship(std::shared_ptr<ast::PrintNode> statementNode) {
  return std::unique_ptr<PrintStmtRelationship>(new PrintStmtRelationship(statementNode));
};

PrintStmtRelationship::PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode) {
  statementNode_ = statementNode;
};

int ReadStmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

EntityType ReadStmtRelationship::entityType() {
  return EntityType::READ_STATEMENT;
};

std::unique_ptr<ReadStmtRelationship> ReadStmtRelationship::CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode) {
  return std::unique_ptr<ReadStmtRelationship>(new ReadStmtRelationship(statementNode));
};

ReadStmtRelationship::ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode) {
  statementNode_ = statementNode;
};
}
