//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsStarRelationship.h"

bool FollowsStarRelationship::containsEntityOnLeftHand(Entity *entity) {
    return this->followedStatement->equals(entity);
}

bool FollowsStarRelationship::containsEntityOnRightHand(Entity *entity) {
    return this->followerStatement->equals(entity);
}

FollowsStarRelationship::FollowsStarRelationship(Statement *followedStatement, Statement *followerStatement) {
    this->followedStatement = followedStatement;
    this->followerStatement = followerStatement;
}

RelationshipType FollowsStarRelationship::getRelationshipType() {
    return RelationshipType::FOLLOWS_STAR;
}

Entity* FollowsStarRelationship::getLeftHandEntity() {
    return this->followedStatement;
}

Entity* FollowsStarRelationship::getRightHandEntity() {
    return this->followerStatement;
}

