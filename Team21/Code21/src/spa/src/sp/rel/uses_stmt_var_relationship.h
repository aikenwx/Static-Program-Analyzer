#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesStmtVarRelationship : public StmtVarRelationship {
 public:
  int statementNumber() override;
  int entityType() override;
  std::string variableName() override;
  static UsesStmtVarRelationship CreateRelationship(
      ast::StatementNode* statementNode, std::string variableName);

 private:
  UsesStmtVarRelationship(ast::StatementNode* statementNode,
                          std::string variableName);
  ast::StatementNode* statementNode_;
  std::string variableName_;
};
}  // namespace rel
