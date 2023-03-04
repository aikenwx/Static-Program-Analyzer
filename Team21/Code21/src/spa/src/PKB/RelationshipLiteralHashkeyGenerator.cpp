//
// Created by Aiken Wong on 11/2/23.
//

#include "RelationshipLiteralHashkeyGenerator.h"

#include <memory>

#include "RelationshipSynonymHashkeyGenerator.h"

std::string RelationshipLiteralHashkeyGenerator::getHashKey(Relationship* relationship) {
    EntityType leftHandEntityType = relationship->getLeftHandEntity()->getEntityType();
    EntityType rightHandEntityType = relationship->getRightHandEntity()->getEntityType();
    std::string* leftHandEntityValue = relationship->getLeftHandEntity()->getEntityValue();
    std::string* rightHandEntityValue = relationship->getRightHandEntity()->getEntityValue();

    return getHashKey(relationship->getRelationshipType(), leftHandEntityType, leftHandEntityValue, rightHandEntityType, rightHandEntityValue);
}

std::string RelationshipLiteralHashkeyGenerator::getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType, std::string* leftHandEntityValue, EntityType rightHandEntityType, std::string* rightHandEntityValue) {
    std::string hashKey = std::string();

    // For simplicity, we will treat all statement types as the same (since statement numbers are unique)
    if (Entity::isStatementType(leftHandEntityType)) {
        leftHandEntityType = EntityType::STATEMENT;
    }
    if (Entity::isStatementType(rightHandEntityType)) {
        rightHandEntityType = EntityType::STATEMENT;
    }

    RelationshipSynonymHashkeyGenerator relationshipSynonymHashkeyGenerator;
    hashKey.append(*leftHandEntityValue);
    hashKey.append("_");
    hashKey.append(std::to_string(relationshipSynonymHashkeyGenerator.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType)));
    hashKey.append("_");
    hashKey.append(*rightHandEntityValue);

    return hashKey;
}
