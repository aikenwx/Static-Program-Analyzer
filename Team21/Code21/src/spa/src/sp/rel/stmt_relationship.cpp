#include "stmt_relationship.h"

namespace rel {
std::unique_ptr<AssignStmtRelationship> AssignStmtRelationship::CreateRelationship(std::shared_ptr<ast::AssignNode> statementNode) {
  return std::unique_ptr<AssignStmtRelationship>(new AssignStmtRelationship(statementNode));
};

AssignStmtRelationship::AssignStmtRelationship(std::shared_ptr<ast::AssignNode> statementNode) {
  statementNode_ = statementNode;
};

std::unique_ptr<CallStmtRelationship> CallStmtRelationship::CreateRelationship(std::shared_ptr<ast::CallNode> statementNode) {
  return std::unique_ptr<CallStmtRelationship>(new CallStmtRelationship(statementNode));
};

CallStmtRelationship::CallStmtRelationship(std::shared_ptr<ast::CallNode> statementNode) {
  statementNode_ = statementNode;
};

std::unique_ptr<IfStmtRelationship> IfStmtRelationship::CreateRelationship(std::shared_ptr<ast::IfNode> statementNode) {
  return std::unique_ptr<IfStmtRelationship>(new IfStmtRelationship(statementNode));
};

IfStmtRelationship::IfStmtRelationship(std::shared_ptr<ast::IfNode> statementNode) {
  statementNode_ = statementNode;
};

std::unique_ptr<WhileStmtRelationship> WhileStmtRelationship::CreateRelationship(std::shared_ptr<ast::WhileNode> statementNode) {
  return std::unique_ptr<WhileStmtRelationship>(new WhileStmtRelationship(statementNode));
};

WhileStmtRelationship::WhileStmtRelationship(std::shared_ptr<ast::WhileNode> statementNode) {
  statementNode_ = statementNode;
};

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
