#pragma once

#include <string>

#include "relationship.h"
#include "../ast/assign_node.h"
#include "../ast/call_node.h"
#include "../ast/if_node.h"
#include "../ast/while_node.h"
#include "../ast/print_node.h"
#include "../ast/read_node.h"

namespace rel {
class AssignStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<AssignStmtRelationship> CreateRelationship(std::shared_ptr<ast::AssignNode> statementNode);

 private:
  AssignStmtRelationship(std::shared_ptr<ast::AssignNode> statementNode);
  std::shared_ptr<ast::AssignNode> statementNode_;
};

class CallStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<CallStmtRelationship> CreateRelationship(std::shared_ptr<ast::CallNode> statementNode);

 private:
  CallStmtRelationship(std::shared_ptr<ast::CallNode> statementNode);
  std::shared_ptr<ast::CallNode> statementNode_;
};

class IfStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<IfStmtRelationship> CreateRelationship(std::shared_ptr<ast::IfNode> statementNode);

 private:
  IfStmtRelationship(std::shared_ptr<ast::IfNode> statementNode);
  std::shared_ptr<ast::IfNode> statementNode_;
};

class WhileStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<WhileStmtRelationship> CreateRelationship(std::shared_ptr<ast::WhileNode> statementNode);

 private:
  WhileStmtRelationship(std::shared_ptr<ast::WhileNode> statementNode);
  std::shared_ptr<ast::WhileNode> statementNode_;
};

class PrintStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<PrintStmtRelationship> CreateRelationship(
      std::shared_ptr<ast::PrintNode> statementNode);

 private:
  PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode);
  std::shared_ptr<ast::PrintNode> statementNode_;
};

class ReadStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<ReadStmtRelationship> CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode);

 private:
  ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode);
  std::shared_ptr<ast::StatementNode> statementNode_;
};
}  // namespace rel
