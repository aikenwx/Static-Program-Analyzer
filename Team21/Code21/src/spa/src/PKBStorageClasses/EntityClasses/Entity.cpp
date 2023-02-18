//
// Created by Aiken Wong on 3/2/23.
//

#include "Entity.h"

std::vector<EntityType> Entity::statementTypes = {
    EntityType::IF_STATEMENT, EntityType::WHILE_STATEMENT, EntityType::ASSIGN_STATEMENT,
    EntityType::CALL_STATEMENT, EntityType::READ_STATEMENT, EntityType::PRINT_STATEMENT};

bool Entity::isStatementType(EntityType entityType) {
    return entityType >= EntityType::ASSIGN_STATEMENT && entityType <= EntityType::WHILE_STATEMENT;
}

bool Entity::equals(Entity *otherEntity) {
    return this->getEntityType() == otherEntity->getEntityType() &&
           *this->getEntityValue() == *otherEntity->getEntityValue();
}
