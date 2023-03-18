//
// Created by Aiken Wong on 3/2/23.
//

#include "ParentRelationship.h"

ParentRelationship::ParentRelationship(Statement *parentStatement, Statement *childStatement) : Relationship(&ParentRelationship::relationshipType, parentStatement, childStatement) {
}

RelationshipType ParentRelationship::relationshipType = RelationshipType();

auto ParentRelationship::getRelationshipTypeStatic() -> RelationshipType & {
  return ParentRelationship::relationshipType;
}

auto ParentRelationship::getRelationshipType() const -> RelationshipType & {
  return ParentRelationship::relationshipType;
}
