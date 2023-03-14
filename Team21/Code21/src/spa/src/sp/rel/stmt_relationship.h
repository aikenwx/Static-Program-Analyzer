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
  static auto CreateRelationship(std::shared_ptr<ast::AssignNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<AssignStmtRelationship>;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::ASSIGN_STMT; };
  auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  AssignStmtRelationship(std::shared_ptr<ast::AssignNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::AssignNode> statementNode_;
};

class CallStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<CallStmtRelationship>;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::CALL_STMT; };
  auto procedureName() const -> std::string { return statementNode_->GetProcedureName(); };
  auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  CallStmtRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::CallNode> statementNode_;
};

class IfStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<IfStmtRelationship>;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::IF_STMT; };
  auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  IfStmtRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::IfNode> statementNode_;
};

class WhileStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<WhileStmtRelationship>;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::WHILE_STMT; };
  auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  WhileStmtRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::WhileNode> statementNode_;
};

class PrintStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<PrintStmtRelationship>;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::PRINT_STMT; };
  auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::PrintNode> statementNode_;
};

class ReadStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<ReadStmtRelationship>;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::READ_STMT; };
  auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode): StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::ReadNode> statementNode_;
};
}  // namespace rel
