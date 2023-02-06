#include "follows_extractor.h"

#include <vector>

#include "../rel/follows_stmt_stmt_relationship.h"
#include "../rel/relationship.h"

namespace design_extractor {
std::optional<std::vector<rel::Relationship>>
FollowsExtractor::HandleStatementListNode(std::vector<ast::INode*> parents,
                                          ast::StatementListNode* node) {
  std::vector<rel::Relationship> relationships;
  std::vector<ast::INode*> statements = *node->GetStatements();
  if (statements.size() <= 1) {
    // no Follows relationship possible in this statement list
    return std::nullopt;
  };

  std::vector<ast::INode*>::const_iterator it = statements.cbegin();
  ast::StatementNode* first = static_cast<ast::StatementNode*>(*it);
  it++;

  for (; it != statements.cend(); it++) {
    ast::StatementNode* second = static_cast<ast::StatementNode*>(*it);
    relationships.push_back(
        rel::FollowsStmtStmtRelationship::CreateRelationship(first, second));
    first = second;
  };

  return relationships;
}

FollowsExtractor* FollowsExtractor::instance_ = nullptr;

FollowsExtractor* FollowsExtractor::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new FollowsExtractor();
  }
  return instance_;
}
}  // namespace design_extractor
