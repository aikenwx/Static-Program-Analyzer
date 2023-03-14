#include "stmt_uses_extractor.h"

#include "../ast/assign_node.h"
#include "../ast/read_node.h"
#include "../ast/statement_node.h"
#include "../ast/variable_node.h"
#include "../rel/relationship.h"
#include "../rel/uses_proc_var_relationship.h"
#include "../rel/uses_stmt_var_relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
void StmtUsesExtractor::UpdateParents(const std::shared_ptr<ast::INode>& node,
                                      int depth) {
  if (depth_ >= depth) {
    // delete all parents below current depth
    parents_.resize(depth + 1);
    // set current-depth parent to `node`
    parents_[depth] = node;
  } else {
    // otherwise, depth_ < depth
    // so we've got a few levels of missing parents???
    // whatever; we'll just pop nullptrs in for now
    while (depth_ < depth - 1) {
      parents_.push_back(nullptr);
      depth_++;
    };
    // at this point, depth_ == depth - 1
    parents_.push_back(node);
    depth_++;
  }
}

void StmtUsesExtractor::HandleVariableNode(
    const std::shared_ptr<ast::VariableNode>& node, int depth) {
  UpdateParents(node, depth);

  // number of tree levels between variable and `*it`
  // start from -1 because `parents` will already have `node`
  int level = -1;
  for (auto it = parents_.rbegin(); it != parents_.rend(); it++) {
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
          relns_.push_back(rel::UsesStmtVarRelationship::CreateRelationship(
              assign, node->GetName()));
        }
        // otherwise `node` is not the RHS, i.e. it's the LHS
        // and so this `node` isn't part of a `Uses` relationship
        break;
      } else {
        // definitely RHS
        relns_.push_back(rel::UsesStmtVarRelationship::CreateRelationship(
            assign, node->GetName()));
      }
    } else if (util::instance_of<ast::ReadNode>(*it)) {
      // `node` is being modified here, not used
      break;
    } else if (util::instance_of<ast::StatementNode>(*it)) {
      // `node` is probably somehow being used here
      relns_.push_back(rel::UsesStmtVarRelationship::CreateRelationship(
          std::static_pointer_cast<ast::StatementNode>(*it), node->GetName()));
      break;
    }
  }
};

auto
StmtUsesExtractor::GetRelationships() const -> std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>> {
  return relns_;
};
}  // namespace design_extractor
