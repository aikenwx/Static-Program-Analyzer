#include "stmt_relationship.h"

namespace rel {
std::unique_ptr<PrintStmtRelationship> PrintStmtRelationship::CreateRelationship(std::shared_ptr<ast::PrintNode> statementNode) {
  return std::unique_ptr<PrintStmtRelationship>(new PrintStmtRelationship(statementNode));
};

PrintStmtRelationship::PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode) {
  statementNode_ = statementNode;
};

std::unique_ptr<ReadStmtRelationship> ReadStmtRelationship::CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode) {
  return std::unique_ptr<ReadStmtRelationship>(new ReadStmtRelationship(statementNode));
};

ReadStmtRelationship::ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode) {
  statementNode_ = statementNode;
};
}
