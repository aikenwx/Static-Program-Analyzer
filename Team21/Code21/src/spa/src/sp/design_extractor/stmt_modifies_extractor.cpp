#include "stmt_modifies_extractor.h"

#include "../ast/assign_node.h"
#include "../ast/read_node.h"
#include "../rel/modifies_proc_var_relationship.h"
#include "../rel/modifies_stmt_var_relationship.h"
#include "../rel/relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
void StmtModifiesExtractor::HandleAssignNode(
    const std::shared_ptr<ast::AssignNode>& node, int depth) {
  relns_.push_back(rel::ModifiesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));
};

void StmtModifiesExtractor::HandleReadNode(const std::shared_ptr<ast::ReadNode>& node,
                                           int depth) {
  relns_.push_back(rel::ModifiesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));
};

std::vector<std::shared_ptr<rel::ModifiesStmtVarRelationship>>
StmtModifiesExtractor::GetRelationships() {
  return relns_;
};
}  // namespace design_extractor
