//
// Created by Aiken Wong on 4/2/23.
//
#include <string>
#include <array>

#include "RelationshipHashFactory.h"

#include "PKBStorageClasses/EntityClasses/Entity.h"



RelationshipHashFactory::RelationshipHashFactory() {

}

std::string RelationshipHashFactory::getHashKey(Relationship *relationship) {



    std::string hashKey = relationshipTypeMapping[relationship->getRelationshipType()] +
    "_" + entityTypeMapping[relationship->getLeftHandEntity()->getEntityType()] +
    "_" + entityTypeMapping[relationship->getRightHandEntity()->getEntityType()];

    return hashKey;
}


std::string RelationshipHashFactory::getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType, EntityType rightHandEntityType) {

    std::string hashKey = relationshipTypeMapping[relationshipType] +
    "_" + entityTypeMapping[leftHandEntityType] +
    "_" + entityTypeMapping[rightHandEntityType];

    return hashKey;
}
