#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ParentStmtStmtRelationship : public StmtStmtRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement) -> std::unique_ptr<ParentStmtStmtRelationship>;
  auto firstStatementNumber() const -> int override;
  auto secondStatementNumber() const -> int override;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::PARENT_STMT_STMT; };

 private:
  ParentStmtStmtRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement
  );
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};
}  // namespace rel
