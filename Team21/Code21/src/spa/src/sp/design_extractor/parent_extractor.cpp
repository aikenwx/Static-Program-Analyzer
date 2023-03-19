#include "parent_extractor.h"

#include "../ast/if_node.h"
#include "../ast/while_node.h"
#include "../rel/parent_stmt_stmt_relationship.h"
#include "../rel/relationship.h"

namespace design_extractor {
void ParentExtractor::HandleIfNode(const std::shared_ptr<ast::IfNode>& node,
                                   int depth) {
  for (const auto& stmt : node->GetThen()->GetStatements()) {
    relns_.push_back(
        rel::ParentStmtStmtRelationship::CreateRelationship(node, stmt));
  }

  for (const auto& stmt : node->GetElse()->GetStatements()) {
    relns_.push_back(
        rel::ParentStmtStmtRelationship::CreateRelationship(node, stmt));
  }
};

void ParentExtractor::HandleWhileNode(const std::shared_ptr<ast::WhileNode>& node,
                                      int depth) {
  for (const auto& stmt : node->GetBody()->GetStatements()) {
    relns_.push_back(
        rel::ParentStmtStmtRelationship::CreateRelationship(node, stmt));
  }
};

auto
ParentExtractor::GetRelationships() const -> std::vector<std::shared_ptr<rel::ParentStmtStmtRelationship>> {
  return relns_;
};
}  // namespace design_extractor
