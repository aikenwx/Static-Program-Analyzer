#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ParentStmtStmtRelationship : public StmtStmtRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement) -> std::unique_ptr<ParentStmtStmtRelationship>;
  [[nodiscard]] auto firstStatementNumber() const -> int override;
  [[nodiscard]] auto secondStatementNumber() const -> int override;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::PARENT_STMT_STMT; };

 private:
  ParentStmtStmtRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement
  ) : firstStatementNode_(std::move(firstStatement)), secondStatementNode_(std::move(secondStatement)) {};
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};
}  // namespace rel
