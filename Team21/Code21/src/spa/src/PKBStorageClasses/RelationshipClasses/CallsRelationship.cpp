//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsRelationship.h"

RelationshipType CallsRelationship::relationshipType = RelationshipType();

auto CallsRelationship::getRelationshipTypeStatic() -> RelationshipType & {
  return CallsRelationship::relationshipType;
}

CallsRelationship::CallsRelationship(Procedure *caller, Procedure *callee) : Relationship(&CallsRelationship::relationshipType, caller, callee) {
}

auto CallsRelationship::getRelationshipType() const -> RelationshipType & {
  return CallsRelationship::getRelationshipTypeStatic();
}
