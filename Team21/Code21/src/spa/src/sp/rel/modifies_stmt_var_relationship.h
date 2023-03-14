#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesStmtVarRelationship : public StmtVarRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, const std::string& variableName) -> std::unique_ptr<ModifiesStmtVarRelationship>;
  [[nodiscard]] auto statementNumber() const -> int override;
  [[nodiscard]] auto variableName() const -> std::string override;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::MODIFIES_STMT_VAR; };

 private:
  ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                              std::string_view variableName) : statementNode_(std::move(statementNode)), variableName_(variableName) {};
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
