//
// Created by Aiken Wong on 3/2/23.
//
#include "ModifiesRelationship.h"


ModifiesRelationship::ModifiesRelationship(Statement *modifier, Variable *modifiedVariable) : Relationship(&ModifiesRelationship::relationshipType, modifier, modifiedVariable) {
}

ModifiesRelationship::ModifiesRelationship(Procedure *modifier, Variable *modifiedVariable) : Relationship(&ModifiesRelationship::relationshipType, modifier, modifiedVariable) {
}

auto ModifiesRelationship::getRelationshipType() const -> const RelationshipType & {
  return ModifiesRelationship::getRelationshipTypeStatic();
}

const RelationshipType ModifiesRelationship::relationshipType = RelationshipType();

auto ModifiesRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return ModifiesRelationship::relationshipType;
}
