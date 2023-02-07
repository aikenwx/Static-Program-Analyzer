#include <string>

#include "modifies_stmt_var_relationship.h"
#include "util.h"

namespace rel {
int ModifiesStmtVarRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};

EntityType ModifiesStmtVarRelationship::entityType() {
  return resolveEntityType(statementNode_);
};

std::string ModifiesStmtVarRelationship::variableName() {
  return variableName_;
};

ModifiesStmtVarRelationship* ModifiesStmtVarRelationship::CreateRelationship(ast::StatementNode* statementNode, std::string variableName) {
  return new ModifiesStmtVarRelationship(statementNode, variableName);
};

ModifiesStmtVarRelationship::ModifiesStmtVarRelationship(ast::StatementNode* statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
