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
  static std::unique_ptr<PrintStmtRelationship> CreateRelationship(
      std::shared_ptr<ast::PrintNode> statementNode);

 private:
  PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode);
  std::shared_ptr<ast::PrintNode> statementNode_;
};

class ReadStmtRelationship : public StmtRelationship {
 public:
  int statementNumber() override;
  EntityType entityType() override;
  static std::unique_ptr<ReadStmtRelationship> CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode);

 private:
  ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode);
  std::shared_ptr<ast::StatementNode> statementNode_;
};
}  // namespace rel
