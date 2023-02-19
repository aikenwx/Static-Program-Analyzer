#include "follows_extractor.h"

#include <vector>

#include "../rel/follows_stmt_stmt_relationship.h"
#include "../rel/relationship.h"

namespace design_extractor {
std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
FollowsExtractor::HandleStatementListNode(std::vector<std::shared_ptr<ast::INode>> parents,
                                          std::shared_ptr<ast::StatementListNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;
  std::vector<std::shared_ptr<ast::INode>> statements = node->GetStatements();
  if (statements.size() <= 1) {
    // no Follows relationship possible in this statement list
    return std::nullopt;
  };

  // nb: `StatementListNode` stores statements in reverse order
  auto it = statements.crbegin();
  std::shared_ptr<ast::StatementNode> first = std::static_pointer_cast<ast::StatementNode>(*it);
  it++;

  for (; it != statements.crend(); it++) {
    std::shared_ptr<ast::StatementNode> second = std::static_pointer_cast<ast::StatementNode>(*it);
    relationships.push_back(
        rel::FollowsStmtStmtRelationship::CreateRelationship(first, second));
    first = second;
  };

  return relationships;
}
}  // namespace design_extractor
