//
// Created by Aiken Wong on 5/2/23.
//

#include "PKBtestHelpers.h"

bool PKBtestHelpers::relationshipEqualsRelationship(Relationship *relationship1, Relationship *relationship2) {
    return relationship1->getRelationshipType() == relationship2->getRelationshipType() &&
           relationship1->getLeftHandEntity()->equals(relationship2->getLeftHandEntity()) &&
           relationship1->getRightHandEntity()->equals(relationship2->getRightHandEntity());
}

void PKBtestHelpers::deleteRelationship(Relationship *relationship) {
    delete relationship->getRightHandEntity();
    delete relationship->getLeftHandEntity();
    delete relationship;
}
