//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsRelationship.h"

const RelationshipType FollowsRelationship::relationshipType = RelationshipType();

FollowsRelationship::FollowsRelationship(Statement *followedStatement, Statement *followerStatement) : Relationship(&FollowsRelationship::relationshipType, followedStatement, followerStatement) {
}

auto FollowsRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return FollowsRelationship::relationshipType;
}

auto FollowsRelationship::getRelationshipType() const -> const RelationshipType & {
  return FollowsRelationship::relationshipType;
}
