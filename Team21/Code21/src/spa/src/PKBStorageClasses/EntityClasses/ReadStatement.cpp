#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber) : Statement(&ReadStatement::getEntityTypeStatic(), statementNumber) {
}

auto ReadStatement::getEntityTypeStatic() -> EntityType& {
  return ReadStatement::readStatementType;
}

auto ReadStatement::getEntityType() const -> EntityType& {
  return ReadStatement::getEntityTypeStatic();
}

EntityType ReadStatement::readStatementType = EntityType();
