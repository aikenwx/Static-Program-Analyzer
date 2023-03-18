//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsStarRelationship.h"

FollowsStarRelationship::FollowsStarRelationship(Statement* followedStatement, Statement* followerStatement) : Relationship(&FollowsStarRelationship::relationshipType, followedStatement, followerStatement) {
}

RelationshipType FollowsStarRelationship::relationshipType = RelationshipType();

auto FollowsStarRelationship::getRelationshipType() const -> RelationshipType& {
  return FollowsStarRelationship::relationshipType;
}

auto FollowsStarRelationship::getRelationshipTypeStatic() -> RelationshipType& {
  return FollowsStarRelationship::relationshipType;
}
