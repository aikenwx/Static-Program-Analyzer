//
// Created by Aiken Wong on 3/2/23.
//

#include "ParentStarRelationship.h"

ParentStarRelationship::ParentStarRelationship(Statement *parentStatement, Statement *childStatement) : Relationship(&ParentStarRelationship::relationshipType, parentStatement, childStatement) {
}

const RelationshipType ParentStarRelationship::relationshipType = RelationshipType();

auto ParentStarRelationship::getRelationshipTypeStatic() -> const RelationshipType & {
  return ParentStarRelationship::relationshipType;
}

auto ParentStarRelationship::getRelationshipType() const -> const RelationshipType & {
  return ParentStarRelationship::relationshipType;
}
