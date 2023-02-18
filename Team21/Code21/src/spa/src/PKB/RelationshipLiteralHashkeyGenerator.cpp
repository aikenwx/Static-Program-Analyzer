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

std::string RelationshipLiteralHashkeyGenerator::getStatementModifiesVariableHashKey(int statementNumber, std::string* variableName) {
    std::string statementNumberString = std::to_string(statementNumber);
    return getHashKey(RelationshipType::MODIFIES, EntityType::STATEMENT, &statementNumberString, EntityType::VARIABLE, variableName);
}

std::string RelationshipLiteralHashkeyGenerator::getProcedureModifiesVariableHashKey(std::string* procedureName, std::string* variableName) {
    return getHashKey(RelationshipType::MODIFIES, EntityType::PROCEDURE, procedureName, EntityType::VARIABLE, variableName);
}

std::string RelationshipLiteralHashkeyGenerator::getStatementUsesVariableHashKey(int statementNumber, std::string* variableName) {
    std::string statementNumberString = std::to_string(statementNumber);

    return getHashKey(RelationshipType::USES, EntityType::STATEMENT, &statementNumberString, EntityType::VARIABLE, variableName);
}

std::string RelationshipLiteralHashkeyGenerator::getProcedureUsesVariableHashKey(std::string* procedureName, std::string* variableName) {
    return getHashKey(RelationshipType::USES, EntityType::PROCEDURE, procedureName, EntityType::VARIABLE, variableName);
}

std::string RelationshipLiteralHashkeyGenerator::getParentRelationshipHashKey(int parentStatementNumber, int childStatementNumber) {
    std::string parentStatementNumberString = std::to_string(parentStatementNumber);
    std::string childStatementNumberString = std::to_string(childStatementNumber);

    return getHashKey(RelationshipType::PARENT, EntityType::STATEMENT, &parentStatementNumberString, EntityType::STATEMENT, &childStatementNumberString);
}

std::string RelationshipLiteralHashkeyGenerator::getFollowsRelationshipHashKey(int firstStatementNumber, int secondStatementNumber) {
    std::string firstStatementNumberString = std::to_string(firstStatementNumber);
    std::string secondStatementNumberString = std::to_string(secondStatementNumber);

    return getHashKey(RelationshipType::FOLLOWS, EntityType::STATEMENT, &firstStatementNumberString, EntityType::STATEMENT, &secondStatementNumberString);
}

std::string RelationshipLiteralHashkeyGenerator::getFollowsStarRelationshipHashKey(int firstStatementNumber, int secondStatementNumber) {
    std::string firstStatementNumberString = std::to_string(firstStatementNumber);
    std::string secondStatementNumberString = std::to_string(secondStatementNumber);

    return getHashKey(RelationshipType::FOLLOWS_STAR, EntityType::STATEMENT, &firstStatementNumberString, EntityType::STATEMENT, &secondStatementNumberString);
}

std::string RelationshipLiteralHashkeyGenerator::getParentStarRelationshipHashKey(int parentStatementNumber, int childStatementNumber) {
    std::string parentStatementNumberString = std::to_string(parentStatementNumber);
    std::string childStatementNumberString = std::to_string(childStatementNumber);

    return getHashKey(RelationshipType::PARENT_STAR, EntityType::STATEMENT, &parentStatementNumberString, EntityType::STATEMENT, &childStatementNumberString);
}
