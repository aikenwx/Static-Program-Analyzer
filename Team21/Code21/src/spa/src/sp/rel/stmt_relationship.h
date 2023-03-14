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
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::ASSIGN_STMT; };
  [[nodiscard]] auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  AssignStmtRelationship(std::shared_ptr<ast::AssignNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::AssignNode> statementNode_;
};

class CallStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<CallStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::CALL_STMT; };
  [[nodiscard]] auto procedureName() const -> std::string { return statementNode_->GetProcedureName(); };
  [[nodiscard]] auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  CallStmtRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::CallNode> statementNode_;
};

class IfStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<IfStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::IF_STMT; };
  [[nodiscard]] auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  IfStmtRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::IfNode> statementNode_;
};

class WhileStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<WhileStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::WHILE_STMT; };
  [[nodiscard]] auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  WhileStmtRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::WhileNode> statementNode_;
};

class PrintStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(
      std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<PrintStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::PRINT_STMT; };
  [[nodiscard]] auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::PrintNode> statementNode_;
};

class ReadStmtRelationship : public StmtRelationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<ReadStmtRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::READ_STMT; };
  [[nodiscard]] auto statementNumber() const -> int override { return statementNode_->GetStatementNumber(); };

 private:
  ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode): StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::ReadNode> statementNode_;
};
}  // namespace rel
