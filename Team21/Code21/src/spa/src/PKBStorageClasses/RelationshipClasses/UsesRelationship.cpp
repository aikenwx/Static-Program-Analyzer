//
// Created by Aiken Wong on 3/2/23.
//
#include "UsesRelationship.h"

#include <stdexcept>

UsesRelationship::UsesRelationship(Entity *user, Variable *usedVariable) : Relationship(&UsesRelationship::getRelationshipType(),
                                                                                        user, usedVariable) {
    if (!Statement::isStatement(user) && dynamic_cast<Procedure *>(user) == nullptr) {
        throw std::invalid_argument("user can only be Statement or Procedure");
    }
}

auto UsesRelationship::getRelationshipType() const -> const RelationshipType & {
  return UsesRelationship::relationshipType;
}

const RelationshipType UsesRelationship::relationshipType = RelationshipType();

auto UsesRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return UsesRelationship::relationshipType;
}
