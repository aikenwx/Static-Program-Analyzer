//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsStarRelationship.h"

FollowsStarRelationship::FollowsStarRelationship(Statement* followedStatement, Statement* followerStatement) : Relationship(&FollowsStarRelationship::relationshipType, followedStatement, followerStatement) {
}

const RelationshipType FollowsStarRelationship::relationshipType = RelationshipType();

auto FollowsStarRelationship::getRelationshipType() const -> const RelationshipType & {
  return FollowsStarRelationship::relationshipType;
}

auto FollowsStarRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return FollowsStarRelationship::relationshipType;
}
