//
// Created by Aiken Wong on 3/2/23.
//
#include "ModifiesRelationship.h"

#include <stdexcept>

ModifiesRelationship::ModifiesRelationship(Entity *modifier, Variable *modifiedVariable) : Relationship(&ModifiesRelationship::relationshipType, modifier, modifiedVariable) {
    // modifier can only be Statement or Procedure
    if (!Statement::isStatement(modifier) && dynamic_cast<Procedure *>(modifier) == nullptr) {
        throw std::invalid_argument("Statement or Procedure expected for first entity of Modifies Relationship");
    }
}

RelationshipType &ModifiesRelationship::getRelationshipType() const {
    return ModifiesRelationship::getRelationshipTypeStatic();
}

RelationshipType ModifiesRelationship::relationshipType = RelationshipType();

RelationshipType &ModifiesRelationship::getRelationshipTypeStatic() {
    return ModifiesRelationship::relationshipType;
}
