//
// Created by Aiken Wong on 27/3/23.
//

#include "AffectsStarRelationship.h"

const RelationshipType AffectsStarRelationship::relationshipType =
    RelationshipType();

auto AffectsStarRelationship::getRelationshipTypeStatic()
    -> const RelationshipType & {
  return AffectsStarRelationship::relationshipType;
}

AffectsStarRelationship::AffectsStarRelationship(AssignStatement *affecter,
                                                 AssignStatement *affectee)
    : Relationship(&AffectsStarRelationship::relationshipType, affecter,
                   affectee) {}

auto AffectsStarRelationship::getRelationshipType() const
    -> const RelationshipType & {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}