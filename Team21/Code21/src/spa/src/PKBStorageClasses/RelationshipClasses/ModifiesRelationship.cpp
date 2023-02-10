//
// Created by Aiken Wong on 3/2/23.
//
#include <stdexcept>
#include <memory>
#include "ModifiesRelationship.h"

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

    this->modifier = std::shared_ptr<Entity>(modifier);
    this->modifiedVariable = std::shared_ptr<Variable>(modifiedVariable);
}

RelationshipType ModifiesRelationship::getRelationshipType() {
    return RelationshipType::MODIFIES;
}

Entity *ModifiesRelationship::getLeftHandEntity() {
    return this->modifier.get();
}

Entity *ModifiesRelationship::getRightHandEntity() {
    return this->modifiedVariable.get();
}

