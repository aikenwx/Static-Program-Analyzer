#include "directly_uses_extractor.h"

#include "../rel/relationship.h"
#include "../rel/uses_stmt_var_relationship.h"
#include "../rel/uses_proc_var_relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
std::optional<std::vector<rel::Relationship>>
DirectlyUsesExtractor::HandlePrintNode(std::vector<ast::INode*> parents,
                                       ast::PrintNode* node) {
  std::vector<rel::Relationship> relationships;

  relationships.push_back(rel::UsesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));

  for (auto it = parents.rbegin(); it != parents.rend(); it++) {
    if (util::instance_of<ast::ProcedureNode>(*it)) {
      relationships.push_back(
          rel::UsesProcVarRelationship::CreateRelationship(
              static_cast<ast::ProcedureNode*>(*it)->GetName(),
              node->GetVariable()->GetName()));
      break; // no nested procedures
    }
    // no container statements (if, while) yet
  }
  return relationships;
}

DirectlyUsesExtractor* DirectlyUsesExtractor::instance_ = nullptr;

DirectlyUsesExtractor* DirectlyUsesExtractor::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new DirectlyUsesExtractor();
  }
  return instance_;
}
}  // namespace design_extractor
