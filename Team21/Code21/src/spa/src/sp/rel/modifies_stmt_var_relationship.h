#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesStmtVarRelationship : public StmtVarRelationship {
 public:
  static std::unique_ptr<ModifiesStmtVarRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, std::string variableName);
  int statementNumber() const override;
  std::string variableName() const override;
  RelationshipType relationshipType() const override { return RelationshipType::MODIFIES_STMT_VAR; };

 private:
  ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                              std::string variableName);
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
