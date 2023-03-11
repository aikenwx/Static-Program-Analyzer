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
  static std::unique_ptr<AssignStmtRelationship> CreateRelationship(std::shared_ptr<ast::AssignNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode);
  RelationshipType relationshipType() override { return RelationshipType::ASSIGN_STMT; };
  int statementNumber() override { return statementNode_->GetStatementNumber(); };

 private:
  AssignStmtRelationship(std::shared_ptr<ast::AssignNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::AssignNode> statementNode_;
};

class CallStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<CallStmtRelationship> CreateRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode);
  RelationshipType relationshipType() override { return RelationshipType::CALL_STMT; };
  std::string procedureName() const { return statementNode_->GetProcedureName(); };
  int statementNumber() override { return statementNode_->GetStatementNumber(); };

 private:
  CallStmtRelationship(std::shared_ptr<ast::CallNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::CallNode> statementNode_;
};

class IfStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<IfStmtRelationship> CreateRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode);
  RelationshipType relationshipType() override { return RelationshipType::IF_STMT; };
  int statementNumber() override { return statementNode_->GetStatementNumber(); };

 private:
  IfStmtRelationship(std::shared_ptr<ast::IfNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::IfNode> statementNode_;
};

class WhileStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<WhileStmtRelationship> CreateRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode);
  RelationshipType relationshipType() override { return RelationshipType::WHILE_STMT; };
  int statementNumber() override { return statementNode_->GetStatementNumber(); };

 private:
  WhileStmtRelationship(std::shared_ptr<ast::WhileNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::WhileNode> statementNode_;
};

class PrintStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<PrintStmtRelationship> CreateRelationship(
      std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode);
  RelationshipType relationshipType() override { return RelationshipType::PRINT_STMT; };
  int statementNumber() override { return statementNode_->GetStatementNumber(); };

 private:
  PrintStmtRelationship(std::shared_ptr<ast::PrintNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode) : StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::PrintNode> statementNode_;
};

class ReadStmtRelationship : public StmtRelationship {
 public:
  static std::unique_ptr<ReadStmtRelationship> CreateRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode);
  RelationshipType relationshipType() override { return RelationshipType::READ_STMT; };
  int statementNumber() override { return statementNode_->GetStatementNumber(); };

 private:
  ReadStmtRelationship(std::shared_ptr<ast::ReadNode> statementNode, std::shared_ptr<ast::ProcedureNode> procedureNode): StmtRelationship(procedureNode), statementNode_(statementNode) {};
  std::shared_ptr<ast::ReadNode> statementNode_;
};
}  // namespace rel
