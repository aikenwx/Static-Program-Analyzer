#include "follows_stmt_stmt_relationship.h"

#include <string>
#include <utility>

namespace rel {
auto FollowsStmtStmtRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> firstStatementNode,
    std::shared_ptr<ast::StatementNode> secondStatementNode)
    -> std::unique_ptr<FollowsStmtStmtRelationship> {
  return std::unique_ptr<FollowsStmtStmtRelationship>(
      new FollowsStmtStmtRelationship(std::move(firstStatementNode),
                                      std::move(secondStatementNode)));
};
} // namespace rel
