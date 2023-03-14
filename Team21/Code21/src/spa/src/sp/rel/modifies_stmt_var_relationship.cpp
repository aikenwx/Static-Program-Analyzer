#include <string>
#include <utility>

#include "modifies_stmt_var_relationship.h"

namespace rel {
auto ModifiesStmtVarRelationship::statementNumber() const -> int {
  return statementNode_->GetStatementNumber();
};

auto ModifiesStmtVarRelationship::variableName() const -> std::string {
  return variableName_;
};

auto ModifiesStmtVarRelationship::CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode, const std::string& variableName) -> std::unique_ptr<ModifiesStmtVarRelationship> {
  return std::unique_ptr<ModifiesStmtVarRelationship>(new ModifiesStmtVarRelationship(std::move(statementNode), variableName));
};

ModifiesStmtVarRelationship::ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string_view variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
