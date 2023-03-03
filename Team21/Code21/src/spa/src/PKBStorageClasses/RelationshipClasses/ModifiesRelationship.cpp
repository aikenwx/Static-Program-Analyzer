//
// Created by Aiken Wong on 3/2/23.
//
#include "ModifiesRelationship.h"

#include <stdexcept>

bool ModifiesRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->modifier->equals(entity);
}

bool ModifiesRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->modifiedVariable->equals(entity);
}

ModifiesRelationship::ModifiesRelationship(Entity *modifier, Variable *modifiedVariable) {
    // modifier can only be Statement or Procedure
    if (!Entity::isStatementType(modifier->getEntityType()) && modifier->getEntityType() != EntityType::PROCEDURE) {
        throw std::invalid_argument("Statement or Procedure expected for first entity of Modifies Relationship");
    }

    this->modifier = modifier;
    this->modifiedVariable = modifiedVariable;
}

RelationshipType ModifiesRelationship::getRelationshipType() {
    return RelationshipType::MODIFIES;
}

Entity *ModifiesRelationship::getLeftHandEntity() {
    return this->modifier;
}

Entity *ModifiesRelationship::getRightHandEntity() {
    return this->modifiedVariable;
}
