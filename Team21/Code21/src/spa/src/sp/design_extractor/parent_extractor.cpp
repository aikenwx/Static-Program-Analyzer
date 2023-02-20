#include "parent_extractor.h"

#include "../rel/relationship.h"
#include "../rel/parent_stmt_stmt_relationship.h"

namespace design_extractor {
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> ParentExtractor::HandleIfNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::IfNode> node) {
    std::vector<std::unique_ptr<rel::Relationship>> relationships;
    for (const auto& stmt : node->GetThen()->GetStatements()) {
      relationships.push_back(rel::ParentStmtStmtRelationship::CreateRelationship(node, stmt));
    }

    for (const auto& stmt : node->GetElse()->GetStatements()) {
      relationships.push_back(rel::ParentStmtStmtRelationship::CreateRelationship(node, stmt));
    }
    return relationships;
  };

  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> ParentExtractor::HandleWhileNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::WhileNode> node) {
    std::vector<std::unique_ptr<rel::Relationship>> relationships;
    for (const auto& stmt : node->GetBody()->GetStatements()) {
      relationships.push_back(rel::ParentStmtStmtRelationship::CreateRelationship(node, stmt));
    }
    return relationships;
  }
}  // namespace design_extractor
