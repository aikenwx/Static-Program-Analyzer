#include <string>

#include "modifies_stmt_var_relationship.h"

namespace rel {
int ModifiesStmtVarRelationship::statementNumber() const {
  return statementNode_->GetStatementNumber();
};

std::string ModifiesStmtVarRelationship::variableName() const {
  return variableName_;
};

std::unique_ptr<ModifiesStmtVarRelationship> ModifiesStmtVarRelationship::CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string variableName) {
  return std::unique_ptr<ModifiesStmtVarRelationship>(new ModifiesStmtVarRelationship(statementNode, variableName));
};

ModifiesStmtVarRelationship::ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode, std::string variableName) {
  statementNode_ = statementNode;
  variableName_ = variableName;
}
}  // namespace rel
