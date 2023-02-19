#pragma once

#include <memory>
#include <string>

#include "../ast/procedure_node.h"
#include "../ast/statement_node.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace rel {
class Relationship {
  public:
    virtual ~Relationship() = default;
};

class StmtVarRelationship : public Relationship {
 public:
  virtual int statementNumber() = 0;
  // TODO: return PKBStorageClasses/EntityClasses/Entity.h?
  virtual std::string variableName() = 0;
  // static StmtVarRelationship CreateRelationship(
  //     std::shared_ptr<ast::StatementNode> statement, std::string variableName);

 private:
  std::shared_ptr<ast::StatementNode> statementNode_;
  std::string variableName_;
};

class ProcVarRelationship : public Relationship {
 public:
  virtual std::string procedureName() = 0;
  virtual std::string variableName() = 0;
  // static ProcVarRelationship CreateRelationship(
  //     std::string procedureName, std::string variableName);

 private:
  std::string procedureName_;
  std::string variableName_;
};

class StmtStmtRelationship : public Relationship {
 public:
  virtual int firstStatementNumber() = 0;
  virtual int secondStatementNumber() = 0;
  // static StmtStmtRelationship CreateRelationship(
  //     std::shared_ptr<ast::StatementNode> firstStatement, std::shared_ptr<ast::StatementNode>
  //     secondStatement);

 private:
  std::shared_ptr<ast::StatementNode> firstStatementNode_;
  std::shared_ptr<ast::StatementNode> secondStatementNode_;
};

class StmtRelationship : public Relationship {
 public:
  int statementNumber();
  // static StmtRelationship CreateRelationship(std::shared_ptr<ast::StatementNode> statement);

 private:
  std::shared_ptr<ast::StatementNode> statementNode_;
};
}  // namespace rel
