#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ParentStmtStmtRelationship : public StmtStmtRelationship {
 public:
  static std::unique_ptr<ParentStmtStmtRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement);
  int firstStatementNumber() const override;
  int secondStatementNumber() const override;
  RelationshipType relationshipType() const override { return RelationshipType::PARENT_STMT_STMT; };

 private:
  ParentStmtStmtRelationship(
      std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode> secondStatement
  );
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};
}  // namespace rel
