//
// Created by Aiken Wong on 3/2/23.
//

#include "ParentStarRelationship.h"

bool ParentStarRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->parentStatement->equals(entity);
}

bool ParentStarRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->childStatement->equals(entity);
}

ParentStarRelationship::ParentStarRelationship(Statement *parentStatement, Statement *childStatement) {
    this->childStatement = childStatement;
    this->parentStatement = parentStatement;
}

RelationshipType ParentStarRelationship::getRelationshipType() {
    return RelationshipType::PARENT_STAR;
}

Entity* ParentStarRelationship::getLeftHandEntity() {
    return this->parentStatement;
}

Entity* ParentStarRelationship::getRightHandEntity() {
    return this->childStatement;
}
