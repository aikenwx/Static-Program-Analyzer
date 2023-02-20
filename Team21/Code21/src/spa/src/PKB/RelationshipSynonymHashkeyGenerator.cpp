//
// Created by Aiken Wong on 4/2/23.
//
#include "RelationshipSynonymHashkeyGenerator.h"

#include <string>

#include "PKBStorageClasses/EntityClasses/Entity.h"

int RelationshipSynonymHashkeyGenerator::getHashKey(Relationship *relationship) {
    return this->getHashKey(relationship->getRelationshipType(), relationship->getLeftHandEntity()->getEntityType(),
                            relationship->getRightHandEntity()->getEntityType());
}

int RelationshipSynonymHashkeyGenerator::getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType,
                                                    EntityType rightHandEntityType) {
    // the base is the maximum of the last entity and last relationship, plus 1, so as to prevent overlap
    int base = std::max<int>(LAST_ENTITY, LAST_RELATIONSHIP) + 1;

    // relationshipType is the most significant digit of the given base
    // leftHandEntityType is the second most significant digit of the given base
    // rightHandEntityType is the least significant digit of the given base

    return (int)relationshipType * base * base + (int)leftHandEntityType * base + (int)rightHandEntityType;
}