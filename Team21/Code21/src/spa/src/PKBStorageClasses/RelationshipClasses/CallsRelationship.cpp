//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsRelationship.h"

const RelationshipType CallsRelationship::relationshipType = RelationshipType();

auto CallsRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return CallsRelationship::relationshipType;
}

CallsRelationship::CallsRelationship(Procedure *caller, Procedure *callee) : Relationship(&CallsRelationship::relationshipType, caller, callee) {
}

auto CallsRelationship::getRelationshipType() const -> const RelationshipType & {
  return CallsRelationship::getRelationshipTypeStatic();
}
