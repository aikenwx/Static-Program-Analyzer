//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsRelationship.h"

RelationshipType FollowsRelationship::relationshipType = RelationshipType();

FollowsRelationship::FollowsRelationship(Statement *followedStatement, Statement *followerStatement) : Relationship(&FollowsRelationship::relationshipType, followedStatement, followerStatement) {
}

RelationshipType &FollowsRelationship::getRelationshipTypeStatic() {
    return FollowsRelationship::relationshipType;
}

RelationshipType &FollowsRelationship::getRelationshipType() const {
    return FollowsRelationship::relationshipType;
}
