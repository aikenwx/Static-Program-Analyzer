//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsStarRelationship.h"

FollowsStarRelationship::FollowsStarRelationship(Statement* followedStatement, Statement* followerStatement) : Relationship(&FollowsStarRelationship::relationshipType, followedStatement, followerStatement) {
}

RelationshipType FollowsStarRelationship::relationshipType = RelationshipType();

RelationshipType& FollowsStarRelationship::getRelationshipType() const {
    return FollowsStarRelationship::relationshipType;
}

RelationshipType& FollowsStarRelationship::getRelationshipTypeStatic() {
    return FollowsStarRelationship::relationshipType;
}
