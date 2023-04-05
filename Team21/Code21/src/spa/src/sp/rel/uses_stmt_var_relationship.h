#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesStmtVarRelationship : public StmtVarRelationship {
public:
  static auto
  CreateRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                     const std::string &variableName)
      -> std::unique_ptr<UsesStmtVarRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override {
    return RelationshipType::USES_STMT_VAR;
  };

private:
  UsesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                          std::string_view variableName)
      : StmtVarRelationship(std::move(statementNode), variableName){};
};
} // namespace rel
