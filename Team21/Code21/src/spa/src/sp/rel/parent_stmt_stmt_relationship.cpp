#include <string>
#include <utility>

#include "parent_stmt_stmt_relationship.h"

namespace rel {
auto ParentStmtStmtRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> firstStatement,
    std::shared_ptr<ast::StatementNode> secondStatement)
    -> std::unique_ptr<ParentStmtStmtRelationship> {
  return std::unique_ptr<ParentStmtStmtRelationship>(
      new ParentStmtStmtRelationship(std::move(firstStatement),
                                     std::move(secondStatement)));
};
} // namespace rel
