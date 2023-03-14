#include "ast_elem_extractor.h"

#include "../ast/astlib.h"
#include "../rel/const_relationship.h"
#include "../rel/proc_relationship.h"
#include "../rel/relationship.h"
#include "../rel/stmt_relationship.h"
#include "../rel/var_relationship.h"

namespace design_extractor {
void AstElemExtractor::HandleAssignNode(
    const std::shared_ptr<ast::AssignNode>& assign_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(
      rel::AssignStmtRelationship::CreateRelationship(assign_node, currentProc_));
}

void AstElemExtractor::HandleCallNode(const std::shared_ptr<ast::CallNode>& call_node,
                                      int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::CallStmtRelationship::CreateRelationship(call_node, currentProc_));
}

void AstElemExtractor::HandleIfNode(const std::shared_ptr<ast::IfNode>& if_node,
                                    int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::IfStmtRelationship::CreateRelationship(if_node, currentProc_));
}

void AstElemExtractor::HandleWhileNode(
    const std::shared_ptr<ast::WhileNode>& while_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::WhileStmtRelationship::CreateRelationship(while_node, currentProc_));
}

void AstElemExtractor::HandleConstantNode(
    const std::shared_ptr<ast::ConstantNode>& constant_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(
      rel::ConstRelationship::CreateRelationship(constant_node->GetValue()));
}

void AstElemExtractor::HandlePrintNode(
    const std::shared_ptr<ast::PrintNode>& print_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::PrintStmtRelationship::CreateRelationship(print_node, currentProc_));
}

void AstElemExtractor::HandleProcedureNode(
    const std::shared_ptr<ast::ProcedureNode>& procedure_node, int depth) {
  std::vector<std::unique_ptr<rel::Relationship>> vec;
  relns_.push_back(rel::ProcRelationship::CreateRelationship(procedure_node));
  currentProc_ = procedure_node;
}

void AstElemExtractor::HandleReadNode(const std::shared_ptr<ast::ReadNode>& read_node,
                                      int depth) {
  relns_.push_back(rel::ReadStmtRelationship::CreateRelationship(read_node, currentProc_));
}

void AstElemExtractor::HandleVariableNode(
    const std::shared_ptr<ast::VariableNode>& variable_node, int depth) {
  relns_.push_back(
      rel::VarRelationship::CreateRelationship(variable_node->GetName()));
}

std::vector<std::shared_ptr<rel::Relationship>>
AstElemExtractor::GetRelationships() const {
  return relns_;
}
}  // namespace design_extractor
