#include "stmt_modifies_extractor.h"

#include "../ast/assign_node.h"
#include "../ast/read_node.h"
#include "../rel/modifies_proc_var_relationship.h"
#include "../rel/modifies_stmt_var_relationship.h"
#include "../rel/relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
StmtModifiesExtractor::HandleAssignNode(
    std::vector<std::shared_ptr<ast::INode>> parents,
    std::shared_ptr<ast::AssignNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  relationships.push_back(rel::ModifiesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));

  return relationships;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
StmtModifiesExtractor::HandleReadNode(
    std::vector<std::shared_ptr<ast::INode>> parents,
    std::shared_ptr<ast::ReadNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  relationships.push_back(rel::ModifiesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));

  return relationships;
}
}  // namespace design_extractor
