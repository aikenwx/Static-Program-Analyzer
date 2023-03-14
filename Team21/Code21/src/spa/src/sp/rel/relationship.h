#pragma once

#include <memory>
#include <string>

#include "../ast/procedure_node.h"
#include "../ast/statement_node.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace rel {
enum class RelationshipType {
  USES_PROC_VAR,
  USES_STMT_VAR,
  MODIFIES_PROC_VAR,
  MODIFIES_STMT_VAR,
  FOLLOWS_STMT_STMT,
  PARENT_STMT_STMT,
  ASSIGN_STMT,
  CALL_STMT,
  IF_STMT,
  WHILE_STMT,
  PRINT_STMT,
  READ_STMT,
  CONST,
  PROC,
  VAR,
  ASSIGN_EXP,
};

class Relationship {
 public:
  virtual ~Relationship() = default;
  virtual auto relationshipType() const -> RelationshipType = 0;
};

class StmtVarRelationship : public Relationship {
 public:
  virtual auto statementNumber() const -> int = 0;
  virtual auto variableName() const -> std::string = 0;

 private:
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};

class ProcVarRelationship : public Relationship {
 public:
  virtual auto procedureName() const -> std::string = 0;
  virtual auto variableName() const -> std::string = 0;

 private:
  std::string procedureName_;
  std::string variableName_;
};

class StmtStmtRelationship : public Relationship {
 public:
  virtual auto firstStatementNumber() const -> int = 0;
  virtual auto secondStatementNumber() const -> int = 0;

 private:
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};

class StmtRelationship : public Relationship {
 public:
  auto parentProcedureName() const -> std::string { return procedureNode_->GetName(); };
  virtual auto statementNumber() const -> int = 0;

 protected:
  explicit StmtRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) : procedureNode_(procedureNode) {};

 private:
  std::shared_ptr<ast::ProcedureNode> procedureNode_;
  // also, some sort of StatementNode, but in child classes
};
}  // namespace rel
