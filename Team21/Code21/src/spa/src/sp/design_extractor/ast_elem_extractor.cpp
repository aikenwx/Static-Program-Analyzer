#include "ast_elem_extractor.h"

#include "../ast/astlib.h"
#include "../rel/const_relationship.h"
#include "../rel/proc_relationship.h"
#include "../rel/relationship.h"
#include "../rel/stmt_relationship.h"
#include "../rel/var_relationship.h"

namespace design_extractor {
void AstElemExtractor::HandleAssignNode(
    std::shared_ptr<ast::AssignNode> assign_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(
      rel::AssignStmtRelationship::CreateRelationship(assign_node));
}

void AstElemExtractor::HandleCallNode(std::shared_ptr<ast::CallNode> call_node,
                                      int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::CallStmtRelationship::CreateRelationship(call_node));
}

void AstElemExtractor::HandleIfNode(std::shared_ptr<ast::IfNode> if_node,
                                    int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::IfStmtRelationship::CreateRelationship(if_node));
}

void AstElemExtractor::HandleWhileNode(
    std::shared_ptr<ast::WhileNode> while_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::WhileStmtRelationship::CreateRelationship(while_node));
}

void AstElemExtractor::HandleConstantNode(
    std::shared_ptr<ast::ConstantNode> constant_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(
      rel::ConstRelationship::CreateRelationship(constant_node->GetValue()));
}

void AstElemExtractor::HandlePrintNode(
    std::shared_ptr<ast::PrintNode> print_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::PrintStmtRelationship::CreateRelationship(print_node));
}

void AstElemExtractor::HandleProcedureNode(
    std::shared_ptr<ast::ProcedureNode> procedure_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::ProcRelationship::CreateRelationship(procedure_node));
}

void AstElemExtractor::HandleReadNode(std::shared_ptr<ast::ReadNode> read_node,
                                      int depth) {
  relns_.push_back(rel::ReadStmtRelationship::CreateRelationship(read_node));
}

void AstElemExtractor::HandleVariableNode(
    std::shared_ptr<ast::VariableNode> variable_node, int depth) {
  relns_.push_back(
      rel::VarRelationship::CreateRelationship(variable_node->GetName()));
}

std::vector<std::shared_ptr<rel::Relationship>>
AstElemExtractor::GetRelationships() {
  return relns_;
}
}  // namespace design_extractor
