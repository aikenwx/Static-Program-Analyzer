#include <string>

#include "uses_stmt_var_relationship.h"
#include "util.h"

namespace rel {
int UsesStmtVarRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

EntityType UsesStmtVarRelationship::entityType() {
  return resolveEntityType(statementNode_);
};

std::string UsesStmtVarRelationship::variableName() {
  return variableName_;
};

UsesStmtVarRelationship* UsesStmtVarRelationship::CreateRelationship(ast::StatementNode* statementNode, std::string variableName) {
  return new UsesStmtVarRelationship(statementNode, variableName);
};

UsesStmtVarRelationship::UsesStmtVarRelationship(ast::StatementNode* statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
