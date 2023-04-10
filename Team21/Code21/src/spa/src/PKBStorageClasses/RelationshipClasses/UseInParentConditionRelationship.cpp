//
// Created by Aiken Wong on 9/4/23.
//

#include "UsesInParentConditionRelationship.h"

UsesInParentConditionRelationship::UsesInParentConditionRelationship(ParentStatement *user, Variable *usedVariable) : Relationship(&UsesInParentConditionRelationship::getRelationshipType(),
                                                                                                                                   user, usedVariable) {
}

auto UsesInParentConditionRelationship::getRelationshipType() const -> const RelationshipType & {
  return UsesInParentConditionRelationship::relationshipType;
}

const RelationshipType UsesInParentConditionRelationship::relationshipType = RelationshipType();

auto UsesInParentConditionRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return UsesInParentConditionRelationship::relationshipType;
}
