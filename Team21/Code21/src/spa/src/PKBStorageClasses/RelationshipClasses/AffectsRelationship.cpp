//
// Created by Aiken Wong on 27/3/23.
//

#include "AffectsRelationship.h"

const RelationshipType AffectsRelationship::relationshipType = RelationshipType();

auto AffectsRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return AffectsRelationship::relationshipType;
}

AffectsRelationship::AffectsRelationship(AssignStatement *affecter, AssignStatement *affectee) : Relationship(&AffectsRelationship::relationshipType, affecter, affectee) {
}

auto AffectsRelationship::getRelationshipType() const -> const RelationshipType & {
  return AffectsRelationship::getRelationshipTypeStatic();
}
