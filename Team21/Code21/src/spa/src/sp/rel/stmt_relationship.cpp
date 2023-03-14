#include "stmt_relationship.h"

#include <utility>

namespace rel {
std::unique_ptr<AssignStmtRelationship> AssignStmtRelationship::CreateRelationship(std::shared_ptr<ast::AssignNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<AssignStmtRelationship>(new AssignStmtRelationship(std::move(statementNode), std::move(procedureNode)));
};

std::unique_ptr<CallStmtRelationship> CallStmtRelationship::CreateRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<CallStmtRelationship>(new CallStmtRelationship(std::move(statementNode), std::move(procedureNode)));
};

std::unique_ptr<IfStmtRelationship> IfStmtRelationship::CreateRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<IfStmtRelationship>(new IfStmtRelationship(std::move(statementNode), std::move(procedureNode)));
};

std::unique_ptr<WhileStmtRelationship> WhileStmtRelationship::CreateRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<WhileStmtRelationship>(new WhileStmtRelationship(std::move(statementNode), std::move(procedureNode)));
};

std::unique_ptr<PrintStmtRelationship> PrintStmtRelationship::CreateRelationship(std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<PrintStmtRelationship>(new PrintStmtRelationship(std::move(statementNode), std::move(procedureNode)));
};

std::unique_ptr<ReadStmtRelationship> ReadStmtRelationship::CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<ReadStmtRelationship>(new ReadStmtRelationship(std::move(statementNode), std::move(procedureNode)));
};
}
