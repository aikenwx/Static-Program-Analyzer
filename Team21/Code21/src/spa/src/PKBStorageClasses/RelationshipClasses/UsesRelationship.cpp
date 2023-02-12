//
// Created by Aiken Wong on 3/2/23.
//
#include <stdexcept>

#include "UsesRelationship.h"

bool UsesRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->user->equals(entity);
}

bool UsesRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->usedVariable->equals(entity);
}

UsesRelationship::UsesRelationship(Entity *user, Variable *usedVariable) {

    // user can only be Statement or Procedure
    if (!Entity::isStatementType(user->getEntityType()) && user->getEntityType() != EntityType::PROCEDURE) {
        throw std::invalid_argument("user can only be Statement or Procedure");
    }
    this->user = user;
    this->usedVariable = usedVariable;
}

RelationshipType UsesRelationship::getRelationshipType() {
    return RelationshipType::USES;
}

Entity* UsesRelationship::getLeftHandEntity() {
    return this->user;
}

Entity* UsesRelationship::getRightHandEntity() {
    return this->usedVariable;
}
