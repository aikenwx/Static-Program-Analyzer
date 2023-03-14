//
// Created by Aiken Wong on 9/3/23.
//
#include "Relationship.h"

std::size_t RelationshipType::relationshipTypeKeyCounter = 0;

RelationshipType::RelationshipType() : StorageKey(RelationshipType::relationshipTypeKeyCounter++){};

Relationship::Relationship(RelationshipType *relationshipType, Entity *leftHandEntity, Entity *rightHandEntity) : relationshipKey(relationshipType, &leftHandEntity->getEntityKey(), &rightHandEntity->getEntityKey()) {
    this->leftHandEntity = leftHandEntity;
    this->rightHandEntity = rightHandEntity;
}

bool RelationshipType::operator==(const RelationshipType &otherRelationshipType) const {
    return this->getKey() == otherRelationshipType.getKey();
}


RelationshipKey::RelationshipKey(RelationshipType *relationshipType, EntityKey *leftEntityKey, EntityKey *rightEntityKey) : StorageKey(relationshipType->getKey() ^ leftEntityKey->getKey() ^ rightEntityKey->getKey()) {
    this->relationshipType = relationshipType;
    this->leftEntityKey = leftEntityKey;
    this->rightEntityKey = rightEntityKey;
}

bool RelationshipKey::operator==(const RelationshipKey &otherRelationshipLiteralKey) const {
    return *this->relationshipType == *otherRelationshipLiteralKey.relationshipType &&
           *this->leftEntityKey == *otherRelationshipLiteralKey.leftEntityKey &&
           *this->rightEntityKey == *otherRelationshipLiteralKey.rightEntityKey;
}

bool Relationship::containsEntityOnLeftHand(Entity *entity) {
    return this->getLeftHandEntity()->equals(entity);
}

bool Relationship::containsEntityOnRightHand(Entity *entity) {
    return this->getRightHandEntity()->equals(entity);
}

bool Relationship::equals(Relationship *otherRelationship) {
    return this->getRelationshipKey() == otherRelationship->getRelationshipKey();
}

Entity *Relationship::getLeftHandEntity() const {
    return leftHandEntity;
}

Entity *Relationship::getRightHandEntity() const {
    return rightHandEntity;
}



RelationshipKey &Relationship::getRelationshipKey() {
    return this->relationshipKey;
}

size_t std::hash<RelationshipKey>::operator()(const RelationshipKey &relationshipKey) const {
    return relationshipKey.getKey();
}
