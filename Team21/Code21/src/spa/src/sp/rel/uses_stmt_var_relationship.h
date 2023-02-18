#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesStmtVarRelationship : public StmtVarRelationship {
 public:
  int statementNumber() override;
  EntityType entityType() override;
  std::string variableName() override;
  static std::unique_ptr<UsesStmtVarRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, std::string variableName);

 private:
  UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                          std::string variableName);
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
