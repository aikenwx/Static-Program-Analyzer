//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsStarRelationship.h"

RelationshipType CallsStarRelationship::relationshipType = RelationshipType();

auto CallsStarRelationship::getRelationshipTypeStatic() -> RelationshipType & {
  return CallsStarRelationship::relationshipType;
}

CallsStarRelationship::CallsStarRelationship(Procedure *caller, Procedure *callee) : Relationship(&CallsStarRelationship::relationshipType, caller, callee) {
}

auto CallsStarRelationship::getRelationshipType() const -> RelationshipType & {
  return CallsStarRelationship::getRelationshipTypeStatic();
}
