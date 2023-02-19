#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesStmtVarRelationship : public StmtVarRelationship {
 public:
  static std::unique_ptr<UsesStmtVarRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, std::string variableName);
  int statementNumber() override;
  std::string variableName() override;
  RelationshipType relationshipType() override { return RelationshipType::USES_STMT_VAR; };

 private:
  UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                          std::string variableName);
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
