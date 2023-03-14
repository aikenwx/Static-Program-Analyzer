//
// Created by Aiken Wong on 2/2/23.
//
#include "Statement.h"

Statement::Statement( EntityType *entityType, int statementNumber) : Entity(entityType, std::make_shared<std::string>(std::to_string(statementNumber))) {
    this->statementNumber = statementNumber;
}

int Statement::getStatementNumber() const {
    return this->statementNumber;
}

bool Statement::isStatement(Entity *entity) {
    return dynamic_cast<Statement *>(entity) != nullptr;
}

EntityType &Statement::getEntityTypeStatic() {
    return Statement::statementType;
}

EntityType Statement::statementType = EntityType();

EntityType &Statement::getEntityType() const {
    return Statement::statementType;
}
