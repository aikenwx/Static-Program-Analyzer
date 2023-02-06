#include <string>

#include "modifies_stmt_var_relationship.h"

namespace rel {
int ModifiesStmtVarRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

int ModifiesStmtVarRelationship::entityType() {
  return 0; // TODO
};

std::string ModifiesStmtVarRelationship::variableName() {
  return variableName_;
};

ModifiesStmtVarRelationship ModifiesStmtVarRelationship::CreateRelationship(ast::StatementNode* statementNode, std::string variableName) {
  return ModifiesStmtVarRelationship(statementNode, variableName);
};

ModifiesStmtVarRelationship::ModifiesStmtVarRelationship(ast::StatementNode* statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
