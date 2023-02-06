#include "ast_elem_extractor.h"

#include "../rel/const_relationship.h"
#include "../rel/proc_relationship.h"
#include "../rel/stmt_relationship.h"
#include "../rel/var_relationship.h"
#include "../rel/relationship.h"

namespace design_extractor {
std::optional<std::vector<rel::Relationship>>
AstElemExtractor::HandleConstantNode(std::vector<ast::INode*> parents,
                                     ast::ConstantNode* constant_node) {
  return std::vector<rel::Relationship>{
      rel::ConstRelationship::CreateRelationship(constant_node->GetValue())};
}

std::optional<std::vector<rel::Relationship>> AstElemExtractor::HandlePrintNode(
    std::vector<ast::INode*> parents, ast::PrintNode* print_node) {
  return std::vector<rel::Relationship>{
      rel::PrintStmtRelationship::CreateRelationship(print_node)};
}

std::optional<std::vector<rel::Relationship>> AstElemExtractor::HandleProcedureNode(
    std::vector<ast::INode*> parents, ast::ProcedureNode* procedure_node) {
  return std::vector<rel::Relationship>{
      rel::ProcRelationship::CreateRelationship(procedure_node->GetName())};
}

std::optional<std::vector<rel::Relationship>> AstElemExtractor::HandleReadNode(
    std::vector<ast::INode*> parents, ast::ReadNode* read_node) {
  return std::vector<rel::Relationship>{
      rel::ReadStmtRelationship::CreateRelationship(read_node)};
}

std::optional<std::vector<rel::Relationship>> AstElemExtractor::HandleVariableNode(
    std::vector<ast::INode*> parents, ast::VariableNode* variable_node) {
  return std::vector<rel::Relationship>{
      rel::VarRelationship::CreateRelationship(variable_node->GetName())};
}

AstElemExtractor* AstElemExtractor::instance_ = nullptr;

AstElemExtractor* AstElemExtractor::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new AstElemExtractor();
  }
  return instance_;
}
}  // namespace design_extractor
