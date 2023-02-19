#include "stmt_uses_extractor.h"

#include "../rel/relationship.h"
#include "../rel/uses_stmt_var_relationship.h"
#include "../rel/uses_proc_var_relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
StmtUsesExtractor::HandlePrintNode(std::vector<std::shared_ptr<ast::INode>> parents,
                                       std::shared_ptr<ast::PrintNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  relationships.push_back(rel::UsesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));

  for (auto it = parents.rbegin(); it != parents.rend(); it++) {
    if (util::instance_of<ast::ProcedureNode>(*it)) {
      relationships.push_back(
          rel::UsesProcVarRelationship::CreateRelationship(
              std::static_pointer_cast<ast::ProcedureNode>(*it)->GetName(),
              node->GetVariable()->GetName()));
      break; // no nested procedures
    }
    // no container statements (if, while) yet
  }
  return relationships;
}
}  // namespace design_extractor
