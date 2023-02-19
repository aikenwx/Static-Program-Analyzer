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

std::unique_ptr<UsesStmtVarRelationship> UsesStmtVarRelationship::CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string variableName) {
  return std::unique_ptr<UsesStmtVarRelationship>(new UsesStmtVarRelationship(statementNode, variableName));
};

UsesStmtVarRelationship::UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
