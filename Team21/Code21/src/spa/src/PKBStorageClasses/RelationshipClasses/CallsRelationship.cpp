//
// Created by Aiken Wong on 3/3/23.
//

#include "CallsRelationship.h"

RelationshipType CallsRelationship::relationshipType = RelationshipType();

RelationshipType &CallsRelationship::getRelationshipTypeStatic() {
    return CallsRelationship::relationshipType;
}

CallsRelationship::CallsRelationship(Procedure *caller, Procedure *callee) : Relationship(&CallsRelationship::relationshipType, caller, callee) {
}

RelationshipType &CallsRelationship::getRelationshipType() const {
    return CallsRelationship::getRelationshipTypeStatic();
}
