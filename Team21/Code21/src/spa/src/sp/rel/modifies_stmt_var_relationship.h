#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesStmtVarRelationship : public StmtVarRelationship {
 public:
  int statementNumber() override;
  std::string variableName() override;
  static std::unique_ptr<ModifiesStmtVarRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, std::string variableName);

 private:
  ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                              std::string variableName);
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
