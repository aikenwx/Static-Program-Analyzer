#include <string>
#include <utility>

#include "uses_stmt_var_relationship.h"

namespace rel {
auto UsesStmtVarRelationship::CreateRelationship(
    std::shared_ptr<ast::StatementNode> statementNode,
    const std::string &variableName)
    -> std::unique_ptr<UsesStmtVarRelationship> {
  return std::unique_ptr<UsesStmtVarRelationship>(
      new UsesStmtVarRelationship(std::move(statementNode), variableName));
};
} // namespace rel
