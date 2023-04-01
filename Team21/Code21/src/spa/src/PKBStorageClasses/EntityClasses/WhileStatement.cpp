#include "WhileStatement.h"

auto WhileStatement::getEntityType() const -> const EntityType & {
  return WhileStatement::whileStatementType;
}

WhileStatement::WhileStatement(int statementNumber) : Statement(&WhileStatement::getEntityTypeStatic(), statementNumber) {
}

auto WhileStatement::getEntityTypeStatic() -> const EntityType & {
  return WhileStatement::whileStatementType;
}

const StatementType WhileStatement::whileStatementType = StatementType();
