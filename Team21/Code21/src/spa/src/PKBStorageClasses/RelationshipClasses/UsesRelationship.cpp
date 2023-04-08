//
// Created by Aiken Wong on 3/2/23.
//
#include "UsesRelationship.h"

#include <stdexcept>

UsesRelationship::UsesRelationship(Statement *user, Variable *usedVariable) : Relationship(&UsesRelationship::getRelationshipType(),
                                                                                           user, usedVariable) {
}

UsesRelationship::UsesRelationship(Procedure *user, Variable *usedVariable) : Relationship(&UsesRelationship::getRelationshipType(),
                                                                                           user, usedVariable) {
}

auto UsesRelationship::getRelationshipType() const -> const RelationshipType & {
  return UsesRelationship::relationshipType;
}

const RelationshipType UsesRelationship::relationshipType = RelationshipType();

auto UsesRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return UsesRelationship::relationshipType;
}
