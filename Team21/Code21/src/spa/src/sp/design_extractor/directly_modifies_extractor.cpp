#include "directly_modifies_extractor.h"

#include "../rel/modifies_stmt_var_relationship.h"
#include "../rel/modifies_proc_var_relationship.h"
#include "../rel/relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
DirectlyModifiesExtractor::HandleReadNode(std::vector<std::shared_ptr<ast::INode>> parents,
                                          std::shared_ptr<ast::ReadNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  relationships.push_back(rel::ModifiesStmtVarRelationship::CreateRelationship(
      node, node->GetVariable()->GetName()));

  for (auto it = parents.rbegin(); it != parents.rend(); it++) {
    if (util::instance_of<ast::ProcedureNode>(*it)) {
      relationships.push_back(
          rel::ModifiesProcVarRelationship::CreateRelationship(
              std::static_pointer_cast<ast::ProcedureNode>(*it)->GetName(),
              node->GetVariable()->GetName()));
      break; // no nested procedures
    }
    // no container statements (if, while) yet
  }
  return relationships;
}

DirectlyModifiesExtractor* DirectlyModifiesExtractor::instance_ = nullptr;

DirectlyModifiesExtractor* DirectlyModifiesExtractor::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new DirectlyModifiesExtractor();
  }
  return instance_;
}
}  // namespace design_extractor
