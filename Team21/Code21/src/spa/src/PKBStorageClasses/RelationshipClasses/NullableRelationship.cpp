//
// Created by Aiken Wong on 25/3/23.
//

#include "NullableRelationship.h"
#include "PKBStorageClasses/EntityClasses/NullableEntity.h"


NullableRelationship::NullableRelationship() : Relationship(&NullableRelationship::relationshipType, (Entity*) &NullableEntity::getNullableEntity(), (Entity*) &NullableEntity::getNullableEntity()) {
}

const RelationshipType NullableRelationship::relationshipType = RelationshipType();

const auto NullableRelationship::nullableRelationship = NullableRelationship();

auto NullableRelationship::getNullableRelationship() -> const NullableRelationship & {
    return NullableRelationship::nullableRelationship;
}

auto NullableRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return NullableRelationship::relationshipType;
}

auto NullableRelationship::getRelationshipType() const -> const RelationshipType & {
  return NullableRelationship::relationshipType;
}
