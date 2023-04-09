#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesStmtVarRelationship : public StmtVarRelationship {
public:
  static auto
  CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                     const std::string &variableName)
      -> std::unique_ptr<ModifiesStmtVarRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override {
    return RelationshipType::MODIFIES_STMT_VAR;
  };

private:
  ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                              std::string_view variableName)
      : StmtVarRelationship(std::move(statementNode), variableName){};
};
} // namespace rel
