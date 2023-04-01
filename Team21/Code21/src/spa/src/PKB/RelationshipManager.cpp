//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <memory>
#include <utility>

#include "PKBStorageClasses/RelationshipClasses/CFGEvaluatableRelationshipType.h"

auto RelationshipManager::emptyRelationshipVector = std::vector<Relationship *>();
auto RelationshipManager::emptyEntityVector = std::vector<Entity *>();

void RelationshipManager::storeCFG(std::shared_ptr<cfg::CFG> givenCfg) {
  this->cfg = std::move(givenCfg);
}

RelationshipManager::RelationshipManager(EntityManager *entityManager) :  entityManager(entityManager) {
}

void RelationshipManager::storeRelationship(
    const std::shared_ptr<Relationship> &relationship) {
  this->relationshipStorage.storeRelationship(relationship);
}

auto RelationshipManager::getRelationship(RelationshipKey &key)
    -> Relationship * {
  if (CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(
          *key.getRelationshipType()) &&
      relationshipStorage.getRelationship(key) == nullptr) {
    auto *leftEntity = this->entityManager->getEntity(*key.getLeftEntityKey());
    auto *rightEntity = this->entityManager->getEntity(*key.getRightEntityKey());

    const auto *cfgRelationshipType = static_cast<const CFGEvaluatableRelationshipType *>(
        key.getRelationshipType());

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    evaluator->evaluateAndCacheRelationshipsByGivenEntities(
        leftEntity,
        rightEntity);
  }

  return this->relationshipStorage.getRelationship(key);
}

auto RelationshipManager::getRelationshipsByTypes(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType)
    -> std::vector<Relationship *> * {
  if (CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(
          relationshipType)) {
      auto cfgRelationshipType = static_cast<const CFGEvaluatableRelationshipType *>(
              &relationshipType);

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    evaluator->evaluateAndCacheRelationshipsByEntityTypes(leftHandEntityType, rightHandEntityType);
  }

  auto result = this->relationshipStorage.getRelationshipsByTypes(relationshipType, leftHandEntityType, rightHandEntityType);

  if (result == nullptr) {
    return &emptyRelationshipVector;
  }

  return result;
}

auto RelationshipManager::
    getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
        const RelationshipType &relationshipType,
        const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
        -> std::vector<Entity *> * {
  if (CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(
          relationshipType)) {
    auto cfgRelationshipType = static_cast<const CFGEvaluatableRelationshipType *>(
        &relationshipType);

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    auto rightHandEntity = this->entityManager->getEntity(rightHandEntityKey);
    evaluator->evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(leftHandEntityType, rightHandEntity, true);
  }

  auto result = this->relationshipStorage.getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(relationshipType, leftHandEntityType, rightHandEntityKey);

  if (result == nullptr) {
    return &emptyEntityVector;
  }
  return result;
}

auto RelationshipManager::
    getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
        const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
        const EntityType &rightHandEntityType) -> std::vector<Entity *> * {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  if (CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(
          relationshipType)) {
    auto cfgRelationshipType = static_cast<const CFGEvaluatableRelationshipType *>(
        &relationshipType);

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    auto leftHandEntity = this->entityManager->getEntity(leftHandEntityKey);

    evaluator->evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(rightHandEntityType, leftHandEntity, false);
  }

  auto result = this->relationshipStorage.getEntitiesForGivenRelationshipTypeAndRightHandEntityType(relationshipType, leftHandEntityKey, rightHandEntityType);

  if (result == nullptr) {
    return &emptyEntityVector;
  }
  return result;
}

auto RelationshipManager::getEmptyEntityVector() -> std::vector<Entity *> * {
  return &emptyEntityVector;
}
