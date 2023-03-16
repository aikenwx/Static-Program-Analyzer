#include "PrintStatement.h"

PrintStatement::PrintStatement(int statementNumber) : Statement(&PrintStatement::getEntityTypeStatic(), statementNumber) {
}

auto PrintStatement::getEntityTypeStatic() -> EntityType& {
  return PrintStatement::printStatementType;
}

auto PrintStatement::getEntityType() const -> EntityType& {
  return PrintStatement::getEntityTypeStatic();
}

EntityType PrintStatement::printStatementType = EntityType();
