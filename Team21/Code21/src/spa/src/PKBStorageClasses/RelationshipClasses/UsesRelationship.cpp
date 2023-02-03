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
    if (dynamic_cast<Statement *>(user) == nullptr && dynamic_cast<Procedure *>(user) == nullptr) {
        throw std::invalid_argument("user can only be Statement or Procedure");
    }

    this->user = user;
    this->usedVariable = usedVariable;
}
