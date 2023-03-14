#include <string>

#include "uses_stmt_var_relationship.h"

namespace rel {
int UsesStmtVarRelationship::statementNumber() const {
  return statementNode_->GetStatementNumber();
};

std::string UsesStmtVarRelationship::variableName() const {
  return variableName_;
};

std::unique_ptr<UsesStmtVarRelationship> UsesStmtVarRelationship::CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string variableName) {
  return std::unique_ptr<UsesStmtVarRelationship>(new UsesStmtVarRelationship(statementNode, variableName));
};

UsesStmtVarRelationship::UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
