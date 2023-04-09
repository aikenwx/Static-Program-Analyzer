//
// Created by Aiken Wong on 4/2/23.
//

#include "QueryFacade.h"

#include <utility>

#include "PKB/EntityManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

QueryFacade::QueryFacade(EntityManager *entityManager,
                         RelationshipManager *relationshipManager,
                         PatternManager *patternManager, CFGStorage *cfgManager)
    : entityManager(entityManager),
      relationshipManager(relationshipManager),
      cfgManager(cfgManager),
      patternManager(patternManager) {}

auto QueryFacade::getEntity(const EntityType &entityType, int entityValue)
    -> Entity * {
  EntityKey entityKey = EntityKey(&entityType, entityValue);
  return this->entityManager->getEntity(entityKey);
}

auto QueryFacade::getEntity(const EntityType &entityType,
                            std::string entityValue) -> Entity * {
  EntityKey entityKey = EntityKey(&entityType, &entityValue);
  return this->entityManager->getEntity(entityKey);
}

auto QueryFacade::getEntitiesByType(const EntityType &entityType)
    -> std::vector<Entity *> * {
  return this->entityManager->getEntitiesByType(entityType);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  int leftEntityValue,
                                  const EntityType &rightEntityType,
                                  int rightEntityValue) -> Relationship * {
  auto *leftEntity = this->getEntity(leftEntityType, leftEntityValue);
  auto *rightEntity = this->getEntity(rightEntityType, rightEntityValue);

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto & leftEntityKey = leftEntity->getEntityKey();
  auto & rightEntityKey = rightEntity->getEntityKey();

  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  std::string leftEntityValue,
                                  const EntityType &rightEntityType,
                                  std::string rightEntityValue)
    -> Relationship * {
  auto *leftEntity =
      this->getEntity(leftEntityType, std::move(leftEntityValue));
  auto *rightEntity =
      this->getEntity(rightEntityType, std::move(rightEntityValue));

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto & leftEntityKey = leftEntity->getEntityKey();
  auto & rightEntityKey = rightEntity->getEntityKey();

  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  int leftEntityValue,
                                  const EntityType &rightEntityType,
                                  std::string rightEntityValue)
    -> Relationship * {
  auto *leftEntity = this->getEntity(leftEntityType, leftEntityValue);
  auto *rightEntity =
      this->getEntity(rightEntityType, std::move(rightEntityValue));

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto & leftEntityKey = leftEntity->getEntityKey();
  auto & rightEntityKey = rightEntity->getEntityKey();

  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  std::string leftEntityValue,
                                  const EntityType &rightEntityType,
                                  int rightEntityValue) -> Relationship * {
  auto *leftEntity =
      this->getEntity(leftEntityType, std::move(leftEntityValue));
  auto *rightEntity = this->getEntity(rightEntityType, rightEntityValue);

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto & leftEntityKey = leftEntity->getEntityKey();
  auto & rightEntityKey = rightEntity->getEntityKey();
  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationshipsByTypes(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    const EntityType &rightEntityType) -> std::vector<Relationship *> * {
  return this->relationshipManager->getRelationshipsByTypes(
      relationshipType, leftEntityType, rightEntityType);
}

auto QueryFacade::getLeftEntitiesRelatedToGivenRightEntity(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    const EntityType &rightEntityType, int rightEntityValue)
    -> std::vector<Entity *> * {
  EntityKey rightEntityKey = EntityKey(&rightEntityType, rightEntityValue);

  Entity *rightEntity = this->entityManager->getEntity(rightEntityKey);

  if (rightEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
          relationshipType, leftEntityType, rightEntity->getEntityKey());
}

auto QueryFacade::getLeftEntitiesRelatedToGivenRightEntity(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    const EntityType &rightEntityType, std::string rightEntityValue)
    -> std::vector<Entity *> * {
  EntityKey rightEntityKey = EntityKey(&rightEntityType, &rightEntityValue);

  Entity *rightEntity = this->entityManager->getEntity(rightEntityKey);

  if (rightEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
          relationshipType, leftEntityType, rightEntity->getEntityKey());
}

auto QueryFacade::getRightEntitiesRelatedToGivenLeftEntity(
        const RelationshipType &relationshipType, const EntityType &leftEntityType,
        int statementNumber, const EntityType &rightEntityType)
    -> std::vector<Entity *> * {
  EntityKey leftEntityKey = EntityKey(&leftEntityType, statementNumber);

  Entity *leftEntity = this->entityManager->getEntity(leftEntityKey);

  if (leftEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
          relationshipType, leftEntity->getEntityKey(), rightEntityType);
}

auto QueryFacade::getRightEntitiesRelatedToGivenLeftEntity(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    std::string leftEntityValue, const EntityType &rightEntityType)
    -> std::vector<Entity *> * {
  EntityKey leftEntityKey = EntityKey(&leftEntityType, &leftEntityValue);

  Entity *leftEntity = this->entityManager->getEntity(leftEntityKey);

  if (leftEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
          relationshipType, leftEntity->getEntityKey(), rightEntityType);
}

void QueryFacade::clearCache() {
  this->relationshipManager->clearCache();
}
