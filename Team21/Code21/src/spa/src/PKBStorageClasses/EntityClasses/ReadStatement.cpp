#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber) : Statement(&ReadStatement::getEntityTypeStatic(), statementNumber) {
}

auto ReadStatement::getEntityTypeStatic() -> const EntityType & {
  return ReadStatement::readStatementType;
}

auto ReadStatement::getEntityType() const -> const EntityType & {
  return ReadStatement::getEntityTypeStatic();
}

const EntityType ReadStatement::readStatementType = EntityType();
