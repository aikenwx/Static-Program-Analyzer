#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesStmtVarRelationship : public StmtVarRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, const std::string& variableName) -> std::unique_ptr<UsesStmtVarRelationship>;
  auto statementNumber() const -> int override;
  auto variableName() const -> std::string override;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::USES_STMT_VAR; };

 private:
  UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                          std::string_view variableName) : statementNode_(std::move(statementNode)), variableName_(variableName) {};
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
