//
// Created by Aiken Wong on 2/2/23.
//
#include "Statement.h"

Statement::Statement(const EntityType *entityType, int statementNumber)
    : Entity(entityType,
             std::make_shared<std::string>(std::to_string(statementNumber))),
      statementNumber(statementNumber) {}

auto Statement::getStatementNumber() const -> int {
  return this->statementNumber;
}

auto Statement::isStatement(Entity *entity) -> bool {
  return dynamic_cast<Statement *>(entity) != nullptr;
}

auto Statement::getEntityTypeStatic() -> const EntityType & {
  return Statement::statementType;
}

const EntityType Statement::statementType = EntityType();

auto Statement::getEntityType() const -> const EntityType & {
  return Statement::statementType;
}
