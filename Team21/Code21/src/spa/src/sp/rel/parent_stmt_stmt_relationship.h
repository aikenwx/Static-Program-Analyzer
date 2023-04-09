#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ParentStmtStmtRelationship : public StmtStmtRelationship {
public:
  static auto
  CreateRelationship(std::shared_ptr<ast::StatementNode> firstStatement,
                     std::shared_ptr<ast::StatementNode> secondStatement)
      -> std::unique_ptr<ParentStmtStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override {
    return RelationshipType::PARENT_STMT_STMT;
  };

private:
  ParentStmtStmtRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement,
      std::shared_ptr<ast::StatementNode> secondStatement)
      : StmtStmtRelationship(std::move(firstStatement),
                             std::move(secondStatement)){};
};
} // namespace rel
