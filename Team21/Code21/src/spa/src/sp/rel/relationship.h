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
  virtual RelationshipType relationshipType() const = 0;
};

class StmtVarRelationship : public Relationship {
 public:
  virtual int statementNumber() const = 0;
  virtual std::string variableName() const = 0;

 private:
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};

class ProcVarRelationship : public Relationship {
 public:
  virtual std::string procedureName() const = 0;
  virtual std::string variableName() const = 0;

 private:
  std::string procedureName_;
  std::string variableName_;
};

class StmtStmtRelationship : public Relationship {
 public:
  virtual int firstStatementNumber() const = 0;
  virtual int secondStatementNumber() const = 0;

 private:
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};

class StmtRelationship : public Relationship {
 public:
  std::string parentProcedureName() const { return procedureNode_->GetName(); };
  virtual int statementNumber() const = 0;

 protected:
  explicit StmtRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) : procedureNode_(procedureNode) {};

 private:
  std::shared_ptr<ast::ProcedureNode> procedureNode_;
  // also, some sort of StatementNode, but in child classes
};
}  // namespace rel
