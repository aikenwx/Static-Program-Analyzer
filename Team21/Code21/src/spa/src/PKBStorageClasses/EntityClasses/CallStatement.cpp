#include "CallStatement.h"

#include <utility>

CallStatement::CallStatement(int statementNumber) : Statement(&CallStatement::getEntityTypeStatic(), statementNumber) {
}

auto CallStatement::getEntityTypeStatic() -> const EntityType & {
  return CallStatement::callStatementType;
}

auto CallStatement::getEntityType() const -> const EntityType & {
  return CallStatement::callStatementType;
}

const StatementType CallStatement::callStatementType = StatementType();

void CallStatement::setProcedureName(std::shared_ptr<std::string> procedureNameString) {
    this->procedureName = std::move(procedureNameString);
}

auto CallStatement::getProcedureName() -> std::string* {
  return procedureName.get();
}
