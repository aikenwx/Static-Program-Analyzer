#include "ast_elem_extractor.h"

#include "../rel/const_relationship.h"
#include "../rel/proc_relationship.h"
#include "../rel/stmt_relationship.h"
#include "../rel/var_relationship.h"
#include "../rel/relationship.h"

namespace design_extractor {
std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleAssignNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::AssignNode> assign_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::AssignStmtRelationship::CreateRelationship(assign_node));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleCallNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::CallNode> call_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::CallStmtRelationship::CreateRelationship(call_node));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleIfNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::IfNode> if_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::IfStmtRelationship::CreateRelationship(if_node));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleWhileNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::WhileNode> while_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::WhileStmtRelationship::CreateRelationship(while_node));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
AstElemExtractor::HandleConstantNode(std::vector<std::shared_ptr<ast::INode>> parents,
                                     std::shared_ptr<ast::ConstantNode> constant_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::ConstRelationship::CreateRelationship(constant_node->GetValue()));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandlePrintNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::PrintNode> print_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::PrintStmtRelationship::CreateRelationship(print_node));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleProcedureNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProcedureNode> procedure_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::ProcRelationship::CreateRelationship(procedure_node->GetName()));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleReadNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ReadNode> read_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::ReadStmtRelationship::CreateRelationship(read_node));
  return vec;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>> AstElemExtractor::HandleVariableNode(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::VariableNode> variable_node) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  vec.push_back(rel::VarRelationship::CreateRelationship(variable_node->GetName()));
  return vec;
}
}  // namespace design_extractor
