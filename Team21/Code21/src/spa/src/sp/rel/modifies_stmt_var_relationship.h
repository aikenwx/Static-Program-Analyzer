#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesStmtVarRelationship : public StmtVarRelationship {
 public:
  int statementNumber() override;
  EntityType entityType() override;
  std::string variableName() override;
  static ModifiesStmtVarRelationship CreateRelationship(
      ast::StatementNode* statementNode, std::string variableName);

 private:
  ModifiesStmtVarRelationship(ast::StatementNode* statementNode,
                              std::string variableName);
  ast::StatementNode* statementNode_;
  std::string variableName_;
};
}  // namespace rel
