//
// Created by Aiken Wong on 3/2/23.
//

#include "Entity.h"

bool Entity::isStatementType(EntityType entityType) {
    return entityType >= EntityType::ASSIGN_STATEMENT && entityType <= EntityType::WHILE_STATEMENT;
}

bool Entity::equals(Entity *otherEntity) {
    return this->getEntityType() == otherEntity->getEntityType() &&
           this->getEntityValue() == otherEntity->getEntityValue();
}
