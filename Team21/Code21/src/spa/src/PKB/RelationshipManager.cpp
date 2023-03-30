//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <memory>

#include "PKB/CFGRelationshipEvaluators/AffectsCFGEvaluator.h"
#include "PKB/CFGRelationshipEvaluators/AffectsStarCFGEvaluator.h"
#include "PKB/CFGRelationshipEvaluators/NextCFGEvaluator.h"
#include "PKB/CFGRelationshipEvaluators/NextStarCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CFGEvaluatableRelationshipType.h"
#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"

void RelationshipManager::storeCFG(std::shared_ptr<cfg::CFG> cfg) {
  this->cfg = cfg;
}

RelationshipManager::RelationshipManager(EntityManager *entityManager) : relationshipStorage(RelationshipStorage()), entityManager(entityManager) {
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
    auto cfgRelationshipType = static_cast<const CFGEvaluatableRelationshipType *>(
        key.getRelationshipType());

    auto leftEntity = this->entityManager->getEntity(*key.getLeftEntityKey());
    auto rightEntity = this->entityManager->getEntity(*key.getRightEntityKey());

    CFGRelationshipEvaluator *evaluator;

    if (*key.getRelationshipType() == NextRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (*key.getRelationshipType() == NextStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (*key.getRelationshipType() == AffectsRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (*key.getRelationshipType() == AffectsStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    }


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

    CFGRelationshipEvaluator *evaluator;

    if (relationshipType == NextRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == NextStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == AffectsRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == AffectsStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    }

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

    CFGRelationshipEvaluator *evaluator;

    if (relationshipType == NextRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == NextStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == AffectsRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == AffectsStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    }

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

    CFGRelationshipEvaluator *evaluator;

    if (relationshipType == NextRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == NextStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new NextStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == AffectsRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    } else if (relationshipType == AffectsStarRelationship::getRelationshipTypeStatic()) {
      evaluator = new AffectsStarCFGEvaluator(this->cfg.get(), &this->relationshipStorage, this->entityManager);
    }

    auto leftHandEntity = this->entityManager->getEntity(leftHandEntityKey);
    evaluator->evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(rightHandEntityType, leftHandEntity, false);
  }

  auto result = this->relationshipStorage.getEntitiesForGivenRelationshipTypeAndRightHandEntityType(relationshipType, leftHandEntityKey, rightHandEntityType);

  if (result == nullptr) {
    return &emptyEntityVector;
  }
  return result;
}
