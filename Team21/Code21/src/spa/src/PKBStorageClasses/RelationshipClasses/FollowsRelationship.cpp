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
    this->followedStatement = std::shared_ptr<Statement>(followedStatement);
    this->followerStatement = std::shared_ptr<Statement>(followerStatement);
}

RelationshipType FollowsRelationship::getRelationshipType() {
    return RelationshipType::FOLLOWS;
}

Entity* FollowsRelationship::getLeftHandEntity() {
    return this->followedStatement.get();
}

Entity* FollowsRelationship::getRightHandEntity() {
    return this->followerStatement.get();
}

