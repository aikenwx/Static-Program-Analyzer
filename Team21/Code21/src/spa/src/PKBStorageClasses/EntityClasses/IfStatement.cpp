#include "IfStatement.h"

EntityType IfStatement::ifStatementType = EntityType();

auto IfStatement::getEntityTypeStatic() -> EntityType& {
  return IfStatement::ifStatementType;
}

auto IfStatement::getEntityType() const -> EntityType& {
  return IfStatement::getEntityTypeStatic();
}

IfStatement::IfStatement(int statementNumber) : Statement(&IfStatement::getEntityTypeStatic(), statementNumber) {
}
