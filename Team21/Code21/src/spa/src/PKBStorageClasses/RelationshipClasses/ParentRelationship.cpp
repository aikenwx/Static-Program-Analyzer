//
// Created by Aiken Wong on 3/2/23.
//

#include "ParentRelationship.h"

ParentRelationship::ParentRelationship(Statement *parentStatement, Statement *childStatement) : Relationship(&ParentRelationship::relationshipType, parentStatement, childStatement) {
}

RelationshipType ParentRelationship::relationshipType = RelationshipType();

RelationshipType &ParentRelationship::getRelationshipTypeStatic() {
    return ParentRelationship::relationshipType;
}

RelationshipType &ParentRelationship::getRelationshipType() const {
    return ParentRelationship::relationshipType;
}
