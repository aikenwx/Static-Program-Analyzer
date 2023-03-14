#include <string>
#include <utility>

#include "uses_stmt_var_relationship.h"

namespace rel {
auto UsesStmtVarRelationship::statementNumber() const -> int {
  return statementNode_->GetStatementNumber();
};

auto UsesStmtVarRelationship::variableName() const -> std::string {
  return variableName_;
};

auto UsesStmtVarRelationship::CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode, const std::string& variableName) -> std::unique_ptr<UsesStmtVarRelationship> {
  return std::unique_ptr<UsesStmtVarRelationship>(new UsesStmtVarRelationship(std::move(statementNode), variableName));
};

UsesStmtVarRelationship::UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string_view variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
