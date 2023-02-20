#include "stmt_uses_extractor.h"

#include "../rel/relationship.h"
#include "../rel/uses_proc_var_relationship.h"
#include "../rel/uses_stmt_var_relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
StmtUsesExtractor::HandleVariableNode(
    std::vector<std::shared_ptr<ast::INode>> parents,
    std::shared_ptr<ast::VariableNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  // number of tree levels between variable and `*it`
  int level = 0;
  for (auto it = parents.rbegin(); it != parents.rend(); it++) {
    level++;
    if (util::instance_of<ast::AssignNode>(*it)) {
      std::shared_ptr<ast::AssignNode> assign =
          std::static_pointer_cast<ast::AssignNode>(*it);

      // decide whether the var was in the LHS or RHS
      if (level == 1) {
        // possibly LHS or RHS
        // so, either `node` was in the LHS, or `node` *IS* the RHS
        if (util::instance_of<ast::VariableNode>(assign->GetAssignment()) &&
            std::static_pointer_cast<ast::VariableNode>(assign->GetAssignment())
                    ->GetName() == node->GetName()) {
          // TODO possible to optimize this with ptr equality check?
          relationships.push_back(
              rel::UsesStmtVarRelationship::CreateRelationship(
                  assign, node->GetName()));
        }
        // otherwise `node` is not the RHS, i.e. it's the LHS
        // and so this `node` isn't part of a `Uses` relationship
        break;
      } else {
        // definitely RHS
        relationships.push_back(
            rel::UsesStmtVarRelationship::CreateRelationship(assign,
                                                             node->GetName()));
      }
    } else if (util::instance_of<ast::ReadNode>(*it)) {
      // `node` is being modified here, not used
      break;
    } else if (util::instance_of<ast::StatementNode>(*it)) {
      // `node` is probably somehow being used here
      relationships.push_back(
          rel::UsesStmtVarRelationship::CreateRelationship(
              std::static_pointer_cast<ast::StatementNode>(*it),
              node->GetName()));
      break;
    }
  }
  return relationships;
}
}  // namespace design_extractor
