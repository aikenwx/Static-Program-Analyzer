#include "WhileStatement.h"

auto WhileStatement::getEntityType() const -> EntityType& {
  return WhileStatement::whileStatementType;
}

WhileStatement::WhileStatement(int statementNumber) : Statement(&WhileStatement::getEntityTypeStatic(), statementNumber) {
}

auto WhileStatement::getEntityTypeStatic() -> EntityType& {
  return WhileStatement::whileStatementType;
}

EntityType WhileStatement::whileStatementType = EntityType();
