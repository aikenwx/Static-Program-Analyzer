//
// Created by Aiken Wong on 4/2/23.
//
#include <string>

#include "RelationshipHashkeyFactory.h"

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include <stdexcept>


RelationshipHashkeyFactory::RelationshipHashkeyFactory() {

}

std::string RelationshipHashkeyFactory::getHashKey(Relationship *relationship) {

    return this->getHashKey(relationship->getRelationshipType(), relationship->getLeftHandEntity()->getEntityType(),
                            relationship->getRightHandEntity()->getEntityType());
}


std::string RelationshipHashkeyFactory::getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType,
                                                   EntityType rightHandEntityType) {

    if (leftHandEntityType == EntityType::STATEMENT || rightHandEntityType == EntityType::STATEMENT) {
        throw std::invalid_argument("Statement is not a specific entity type.");
    }

    std::string hashKey = relationshipTypeMapping[relationshipType] +
                          "_" + entityTypeMapping[leftHandEntityType] +
                          "_" + entityTypeMapping[rightHandEntityType];


    return hashKey;
}
