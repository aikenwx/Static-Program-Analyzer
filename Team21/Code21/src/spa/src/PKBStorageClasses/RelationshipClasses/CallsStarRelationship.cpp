//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsStarRelationship.h"

const RelationshipType CallsStarRelationship::relationshipType = RelationshipType();

auto CallsStarRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return CallsStarRelationship::relationshipType;
}

CallsStarRelationship::CallsStarRelationship(Procedure *caller, Procedure *callee) : Relationship(&CallsStarRelationship::relationshipType, caller, callee) {
}

auto CallsStarRelationship::getRelationshipType() const -> const RelationshipType & {
  return CallsStarRelationship::getRelationshipTypeStatic();
}
