//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsRelationship.h"

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
