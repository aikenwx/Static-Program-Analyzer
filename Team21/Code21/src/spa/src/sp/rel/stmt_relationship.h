#pragma once

#include <string>

#include "relationship.h"
#include "../ast/print_node.h"
#include "../ast/read_node.h"

namespace rel {
class PrintStmtRelationship : public StmtRelationship {
 public:
  int statementNumber() override;
  EntityType entityType() override;
  static PrintStmtRelationship CreateRelationship(
      ast::PrintNode* statementNode);

 private:
  PrintStmtRelationship(ast::PrintNode* statementNode);
  ast::PrintNode* statementNode_;
};

class ReadStmtRelationship : public StmtRelationship {
 public:
  int statementNumber() override;
  EntityType entityType() override;
  static ReadStmtRelationship CreateRelationship(ast::ReadNode* statementNode);

 private:
  ReadStmtRelationship(ast::ReadNode* statementNode);
  ast::StatementNode* statementNode_;
};
}  // namespace rel
