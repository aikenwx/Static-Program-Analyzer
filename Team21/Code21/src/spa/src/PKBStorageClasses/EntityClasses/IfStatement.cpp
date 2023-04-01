#include "IfStatement.h"

const StatementType IfStatement::ifStatementType = StatementType();

auto IfStatement::getEntityTypeStatic() -> const EntityType & {
  return IfStatement::ifStatementType;
}

auto IfStatement::getEntityType() const -> const EntityType & {
  return IfStatement::getEntityTypeStatic();
}

IfStatement::IfStatement(int statementNumber) : Statement(&IfStatement::getEntityTypeStatic(), statementNumber) {
}
