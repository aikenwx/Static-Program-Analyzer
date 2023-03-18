//
// Created by Aiken Wong on 9/3/23.
//
#include "Relationship.h"

std::size_t RelationshipType::relationshipTypeKeyCounter = 0;

RelationshipType::RelationshipType() : StorageKey(RelationshipType::relationshipTypeKeyCounter++){};

Relationship::Relationship(const RelationshipType *relationshipType,
                           Entity *leftHandEntity, Entity *rightHandEntity)
    : relationshipKey(relationshipType, &leftHandEntity->getEntityKey(),
                      &rightHandEntity->getEntityKey()),
      leftHandEntity(leftHandEntity),
      rightHandEntity(rightHandEntity) {}

auto RelationshipType::operator==(
    const RelationshipType &otherRelationshipType) const -> bool {
  return this->getKey() == otherRelationshipType.getKey();
}

RelationshipKey::RelationshipKey(const RelationshipType *relationshipType,
                                 EntityKey *leftEntityKey,
                                 EntityKey *rightEntityKey)
    : StorageKey(relationshipType->getKey() ^ leftEntityKey->getKey() ^
                 rightEntityKey->getKey()),
      relationshipType(relationshipType),
      leftEntityKey(leftEntityKey),
      rightEntityKey(rightEntityKey) {}

auto RelationshipKey::operator==(
    const RelationshipKey &otherRelationshipLiteralKey) const -> bool {
  return *this->relationshipType ==
             *otherRelationshipLiteralKey.relationshipType &&
         *this->leftEntityKey == *otherRelationshipLiteralKey.leftEntityKey &&
         *this->rightEntityKey == *otherRelationshipLiteralKey.rightEntityKey;
}

auto Relationship::containsEntityOnLeftHand(Entity *entity) const -> bool {
  return this->getLeftHandEntity()->equals(entity);
}

auto Relationship::containsEntityOnRightHand(Entity *entity) const -> bool {
  return this->getRightHandEntity()->equals(entity);
}

auto Relationship::equals(Relationship *otherRelationship) -> bool {
  return this->getRelationshipKey() == otherRelationship->getRelationshipKey();
}

auto Relationship::getLeftHandEntity() const -> Entity * {
  return leftHandEntity;
}

auto Relationship::getRightHandEntity() const -> Entity * {
  return rightHandEntity;
}

auto Relationship::getRelationshipKey() -> RelationshipKey & {
  return this->relationshipKey;
}

auto std::hash<RelationshipKey>::operator()(
    const RelationshipKey &relationshipKey) const -> size_t {
  return relationshipKey.getKey();
}
