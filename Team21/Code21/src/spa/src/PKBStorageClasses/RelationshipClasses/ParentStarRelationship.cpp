//
// Created by Aiken Wong on 3/2/23.
//

#include "ParentStarRelationship.h"

ParentStarRelationship::ParentStarRelationship(Statement *parentStatement, Statement *childStatement) : Relationship(&ParentStarRelationship::relationshipType, parentStatement, childStatement) {
}

RelationshipType ParentStarRelationship::relationshipType = RelationshipType();

RelationshipType &ParentStarRelationship::getRelationshipTypeStatic() {
    return ParentStarRelationship::relationshipType;
}

RelationshipType &ParentStarRelationship::getRelationshipType() const {
    return ParentStarRelationship::relationshipType;
}
