//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsStarRelationship.h"

RelationshipType CallsStarRelationship::relationshipType = RelationshipType();

RelationshipType &CallsStarRelationship::getRelationshipTypeStatic() {
    return CallsStarRelationship::relationshipType;
}

CallsStarRelationship::CallsStarRelationship(Procedure *caller, Procedure *callee) : Relationship(&CallsStarRelationship::relationshipType, caller, callee) {
}

RelationshipType &CallsStarRelationship::getRelationshipType() const {
    return CallsStarRelationship::getRelationshipTypeStatic();
}
