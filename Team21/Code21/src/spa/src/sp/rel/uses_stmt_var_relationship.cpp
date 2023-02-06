#include <string>

#include "uses_stmt_var_relationship.h"

namespace rel {
int UsesStmtVarRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

int UsesStmtVarRelationship::entityType() {
  return 0; // TODO
};

std::string UsesStmtVarRelationship::variableName() {
  return variableName_;
};

UsesStmtVarRelationship UsesStmtVarRelationship::CreateRelationship(ast::StatementNode* statementNode, std::string variableName) {
  return UsesStmtVarRelationship(statementNode, variableName);
};

UsesStmtVarRelationship::UsesStmtVarRelationship(ast::StatementNode* statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
