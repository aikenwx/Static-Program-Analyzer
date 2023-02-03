//
// Created by Aiken Wong on 3/2/23.
//

#include "ModifiesRelationship.h"

bool ModifiesRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->modifier->equals(entity);
}

bool ModifiesRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->modifiedVariable->equals(entity);
}

ModifiesRelationship::ModifiesRelationship(Entity *modifier, Variable *modifiedVariable) {

    // modifier can only be Statement or Procedure
    if (dynamic_cast<Statement *>(modifier) == nullptr && dynamic_cast<Procedure *>(modifier) == nullptr) {
        throw std::invalid_argument("user can only be Statement or Procedure");
    }

    this->modifier = modifier;
    this->modifiedVariable = modifiedVariable;
}
