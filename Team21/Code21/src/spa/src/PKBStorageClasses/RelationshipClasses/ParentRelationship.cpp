//
// Created by Aiken Wong on 3/2/23.
//

#include "ParentRelationship.h"

bool ParentRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->parentStatement->equals(entity);
}

bool ParentRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->childStatement->equals(entity);
}

ParentRelationship::ParentRelationship(Statement *parentStatement, Statement *childStatement) {
    this->childStatement = childStatement;
    this->parentStatement = parentStatement;
}

RelationshipType ParentRelationship::getRelationshipType() {
    return RelationshipType::PARENT;
}
