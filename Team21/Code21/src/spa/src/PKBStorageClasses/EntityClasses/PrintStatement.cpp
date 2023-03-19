#include "PrintStatement.h"

PrintStatement::PrintStatement(int statementNumber) : Statement(&PrintStatement::getEntityTypeStatic(), statementNumber) {
}

auto PrintStatement::getEntityTypeStatic() -> const EntityType & {
  return PrintStatement::printStatementType;
}

auto PrintStatement::getEntityType() const -> const EntityType & {
  return PrintStatement::getEntityTypeStatic();
}

const EntityType PrintStatement::printStatementType = EntityType();
