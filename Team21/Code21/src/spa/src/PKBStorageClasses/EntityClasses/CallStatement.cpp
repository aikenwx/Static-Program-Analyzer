#include "CallStatement.h"

CallStatement::CallStatement(int statementNumber) : Statement(&CallStatement::getEntityTypeStatic(), statementNumber) {
}

auto CallStatement::getEntityTypeStatic() -> EntityType& {
  return CallStatement::callStatementType;
}

auto CallStatement::getEntityType() const -> EntityType& {
  return CallStatement::callStatementType;
}

EntityType CallStatement::callStatementType = EntityType();

void CallStatement::setProcedureName(std::shared_ptr<std::string> procedureName) {
    this->procedureName = procedureName;
}

auto CallStatement::getProcedureName() -> std::string* {
  return procedureName.get();
}
