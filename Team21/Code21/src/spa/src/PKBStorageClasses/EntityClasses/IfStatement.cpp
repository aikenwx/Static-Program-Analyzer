#include "IfStatement.h"

const EntityType IfStatement::ifStatementType = EntityType();

auto IfStatement::getEntityTypeStatic() -> const EntityType & {
  return IfStatement::ifStatementType;
}

auto IfStatement::getEntityType() const -> const EntityType & {
  return IfStatement::getEntityTypeStatic();
}

IfStatement::IfStatement(int statementNumber) : Statement(&IfStatement::getEntityTypeStatic(), statementNumber) {
}
