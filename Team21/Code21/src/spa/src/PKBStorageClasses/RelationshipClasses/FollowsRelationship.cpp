//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsRelationship.h"
#include <memory>

bool FollowsRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->followedStatement->equals(entity);
}

bool FollowsRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->followerStatement->equals(entity);
}

FollowsRelationship::FollowsRelationship(Statement *followedStatement, Statement *followerStatement) {
    this->followedStatement = followedStatement;
    this->followerStatement = followerStatement;
}

RelationshipType FollowsRelationship::getRelationshipType() {
    return RelationshipType::FOLLOWS;
}

Entity* FollowsRelationship::getLeftHandEntity() {
    return this->followedStatement;
}

Entity* FollowsRelationship::getRightHandEntity() {
    return this->followerStatement;
}

