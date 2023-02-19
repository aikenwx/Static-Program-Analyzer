#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesStmtVarRelationship : public StmtVarRelationship {
 public:
  static std::unique_ptr<ModifiesStmtVarRelationship> CreateRelationship(
      std::shared_ptr<ast::StatementNode> statementNode, std::string variableName);
  int statementNumber() override;
  std::string variableName() override;
  RelationshipType relationshipType() override { return RelationshipType::MODIFIES_STMT_VAR; };

 private:
  ModifiesStmtVarRelationship(std::shared_ptr<ast::StatementNode> statementNode,
                              std::string variableName);
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};
}  // namespace rel
