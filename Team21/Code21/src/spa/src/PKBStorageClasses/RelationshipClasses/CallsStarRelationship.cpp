//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsStarRelationship.h"

bool CallsStarRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->caller->equals(entity);
}

bool CallsStarRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->callee->equals(entity);
}

CallsStarRelationship::CallsStarRelationship(Procedure *caller, Procedure *callee) {
    this->callee = callee;
    this->caller = caller;
}

RelationshipType CallsStarRelationship::getRelationshipType() {
    return RelationshipType::CALLS_STAR;
}

Entity *CallsStarRelationship::getLeftHandEntity() {
    return this->caller;
}

Entity *CallsStarRelationship::getRightHandEntity() {
    return this->callee;
}

