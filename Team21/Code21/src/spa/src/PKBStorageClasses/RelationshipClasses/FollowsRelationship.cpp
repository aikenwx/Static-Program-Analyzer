//
// Created by Aiken Wong on 3/2/23.
//

#include "FollowsRelationship.h"

RelationshipType FollowsRelationship::relationshipType = RelationshipType();

FollowsRelationship::FollowsRelationship(Statement *followedStatement, Statement *followerStatement) : Relationship(&FollowsRelationship::relationshipType, followedStatement, followerStatement) {
}

auto FollowsRelationship::getRelationshipTypeStatic() -> RelationshipType & {
  return FollowsRelationship::relationshipType;
}

auto FollowsRelationship::getRelationshipType() const -> RelationshipType & {
  return FollowsRelationship::relationshipType;
}
