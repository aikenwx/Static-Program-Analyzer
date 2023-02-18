#include "ast_elem_extractor.h"

#include "../rel/const_relationship.h"
#include "../rel/proc_relationship.h"
#include "../rel/stmt_relationship.h"
#include "../rel/var_relationship.h"
#include "../rel/relationship.h"

namespace design_extractor {
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

AstElemExtractor* AstElemExtractor::instance_ = nullptr;

AstElemExtractor* AstElemExtractor::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new AstElemExtractor();
  }
  return instance_;
}
}  // namespace design_extractor
