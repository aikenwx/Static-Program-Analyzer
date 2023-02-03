//
// Created by Aiken Wong on 3/2/23.
//

#include "UsesRelationship.h"
bool UsesRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->user->equals(entity);
}

bool UsesRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->usedVariable->equals(entity);
}

UsesRelationship::UsesRelationship(Entity *user, Variable *usedVariable) {
    this->user = user;
    this->usedVariable = usedVariable;
}
