//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsRelationship.h"


bool CallsRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->caller->equals(entity);
}

bool CallsRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->callee->equals(entity);
}

CallsRelationship::CallsRelationship(Procedure *caller, Procedure *callee ) {
    this->callee = callee;
    this->caller = caller;
}

RelationshipType CallsRelationship::getRelationshipType() {
    return RelationshipType::CALLS;
}

Entity *CallsRelationship::getLeftHandEntity() {
    return this->caller;
}

Entity *CallsRelationship::getRightHandEntity() {
    return this->callee;
}
