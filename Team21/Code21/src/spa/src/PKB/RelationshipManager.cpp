//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <memory>
#include <utility>

#include "PKBStorageClasses/RelationshipClasses/CFGEvaluatableRelationshipType.h"

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::vector<Relationship *> RelationshipManager::emptyRelationshipVector =
    std::vector<Relationship *>();

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::vector<Entity *> RelationshipManager::emptyEntityVector =
    std::vector<Entity *>();

void RelationshipManager::storeCFG(std::shared_ptr<cfg::CFG> givenCfg) {
  this->cfg = std::move(givenCfg);
}

RelationshipManager::RelationshipManager(EntityManager *entityManager)
    : entityManager(entityManager) {}

void RelationshipManager::storeRelationship(
        std::unique_ptr<Relationship> relationship) {
  this->relationshipStorage.storeRelationship(std::move(relationship));
}

auto RelationshipManager::getRelationship(RelationshipKey &key, Entity *leftEntity, Entity *rightEntity)
    -> Relationship * {
  if (CFGEvaluableRelationshipType::isCFGEvaluableRelationship(
          *key.getRelationshipType())) {
    const auto *cfgRelationshipType =
        dynamic_cast<const CFGEvaluableRelationshipType *>(
            key.getRelationshipType());

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    return evaluator->evaluateAndCacheRelationshipsByGivenEntities(leftEntity,
                                                            rightEntity);
  }

  return this->relationshipStorage.getRelationshipFromStore(key);
}

auto RelationshipManager::getRelationshipsByTypes(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType)
    -> std::vector<Relationship *> * {
  if (CFGEvaluableRelationshipType::isCFGEvaluableRelationship(
          relationshipType)) {
    const auto *cfgRelationshipType =
        dynamic_cast<const CFGEvaluableRelationshipType *>(&relationshipType);

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    return evaluator->evaluateAndCacheRelationshipsByEntityTypes(leftHandEntityType,
                                                          rightHandEntityType);
  }

  auto *result = this->relationshipStorage.getRelationshipsByTypesFromStore(
          relationshipType, leftHandEntityType, rightHandEntityType);

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
  if (CFGEvaluableRelationshipType::isCFGEvaluableRelationship(
          relationshipType)) {
    const auto *cfgRelationshipType =
        dynamic_cast<const CFGEvaluableRelationshipType *>(&relationshipType);

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    auto *rightHandEntity = this->entityManager->getEntity(rightHandEntityKey);
    return evaluator->evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        leftHandEntityType, rightHandEntity, true).first;
  }

  auto *result =
          this->relationshipStorage
                  .getEntitiesFromSynonymLiteralStore(
                          relationshipType, leftHandEntityType, rightHandEntityKey);

  if (result == nullptr) {
    return &emptyEntityVector;
  }
  return result;
}

auto RelationshipManager::
    getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
        const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
        const EntityType &rightHandEntityType) -> std::vector<Entity *> * {
  if (CFGEvaluableRelationshipType::isCFGEvaluableRelationship(
          relationshipType)) {
    const auto *cfgRelationshipType =
        dynamic_cast<const CFGEvaluableRelationshipType *>(&relationshipType);

    auto evaluator = cfgRelationshipType->getRelationshipEvaluator(
        this->cfg.get(), &this->relationshipStorage, this->entityManager);

    auto *leftHandEntity = this->entityManager->getEntity(leftHandEntityKey);

    return evaluator->evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        rightHandEntityType, leftHandEntity, false).first;
  }

  auto *result =
          this->relationshipStorage
                  .getEntitiesFromLiteralSynonymStore(
                          relationshipType, leftHandEntityKey, rightHandEntityType);

  if (result == nullptr) {
    return &emptyEntityVector;
  }
  return result;
}

auto RelationshipManager::getEmptyEntityVector() -> std::vector<Entity *> * {
  return &emptyEntityVector;
}

auto RelationshipManager::getEmptyRelationshipVector()
    -> std::vector<Relationship *> * {
  return &emptyRelationshipVector;
}

void RelationshipManager::clearCache() {
  this->relationshipStorage.clearCache();
}

auto RelationshipManager::getStoreAndCacheSizes() -> std::vector<int> {
  return this->relationshipStorage.getStoreAndCacheSizes();
}
