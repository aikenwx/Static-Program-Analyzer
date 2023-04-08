//
// Created by Aiken Wong on 26/3/23.
//

#include "CFGRelationshipEvaluator.h"

#include "PKB/RelationshipManager.h"

std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>
    CFGRelationshipEvaluator::emptyEntityVectorRelationshipVectorPair =
        std::make_pair(RelationshipManager::getEmptyEntityVector(),
                       RelationshipManager::getEmptyRelationshipVector());

CFGRelationshipEvaluator::CFGRelationshipEvaluator(
    cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
    EntityManager *entityManager)
    : cfg(cfg),
      relationshipStorage(relationshipStorage),
      entityManager(entityManager) {}

auto CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<Relationship *> * {
  auto *leftEntityList = getEvaluableEntitiesFromEntityType(leftEntityType);
  auto *rightEntityList = getEvaluableEntitiesFromEntityType(rightEntityType);
  this->entityManager->getEntitiesByType(rightEntityType);

  // if either list is empty, no need to evaluate, relationships cannot exist
  if (leftEntityList->empty() || rightEntityList->empty()) {
    return RelationshipManager::getEmptyRelationshipVector();
  }
  // if contents of either list is not a statement, no need to evaluate, as they
  // are not CFG evaluable Relationships
  if (!Statement::isStatement(leftEntityList->at(0)) ||
      !Statement::isStatement(rightEntityList->at(0))) {
    return RelationshipManager::getEmptyRelationshipVector();
  }

  if (!isValidEntityTypeInput(leftEntityType) ||
      !isValidEntityTypeInput(rightEntityType)) {
    return RelationshipManager::getEmptyRelationshipVector();
  }

  // already evaluated

  auto *possibleCachedRelationships =
      relationshipStorage->getCachedRelationshipsByTypes(
          getRelationshipType(), leftEntityType, rightEntityType);

  if (possibleCachedRelationships != nullptr) {
    return possibleCachedRelationships;
  }

  bool isReverse = shouldEvaluateRelationshipsByEntityTypesInReverse(
      leftEntityType, rightEntityType);

  auto results = std::make_unique<std::vector<Relationship *>>();

  for (auto *entity : isReverse ? *rightEntityList : *leftEntityList) {
    auto *partialResults =
        evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
            isReverse ? leftEntityType : rightEntityType, entity, isReverse)
            .second;

    for (auto *partialResult : *partialResults) {
      results->push_back(partialResult);
    }
  }

  auto *resultsPtr = results.get();
  relationshipStorage->storeInRelationshipDoubleSynonymCache(
      getRelationshipType(), leftEntityType, rightEntityType,
      std::move(results));

  return resultsPtr;
}

auto CFGRelationshipEvaluator::
    evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        const EntityType &givenEntityType, Entity *entity, bool isReverse)
        -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> & {
  auto *entityList = this->entityManager->getEntitiesByType(givenEntityType);

  if (entityList->empty()) {
    return emptyEntityVectorRelationshipVectorPair;
  }

  if (!Statement::isStatement(entityList->at(0)) ||
      !Statement::isStatement(entity)) {
    return emptyEntityVectorRelationshipVectorPair;
  }

  if (!isValidEntityInput(entity) || !isValidEntityTypeInput(givenEntityType)) {
    return emptyEntityVectorRelationshipVectorPair;
  }

  auto *statement = dynamic_cast<Statement *>(entity);

  // already evaluated

  auto &cachedPair =
      getCachedEntitiesAndRelationships(isReverse, *statement, givenEntityType);
  // auto *possibleCachedStatements = cachedPair.first;
  // std::vector<Relationship *> *possibleCachedRelationships =
  // cachedPair.second;

  if (cachedPair.first != nullptr) {
    return cachedPair;
  }

  auto &cachedStmtPair = getCachedEntitiesAndRelationships(
      isReverse, *statement, Statement::getEntityTypeStatic());

  if (cachedStmtPair.first == nullptr) {
    auto relatedStatements = this->getRelatedStatements(statement, isReverse);

    auto ownerResultRelationships =
        std::make_unique<std::vector<Relationship *>>();

    for (auto *relatedStatement : *relatedStatements) {
      auto relationship =
          isReverse ? createNewRelationship(relatedStatement, statement)
                    : createNewRelationship(statement, relatedStatement);

      ownerResultRelationships->push_back(relationship.get());
      relationshipStorage->storeInRelationshipOwnerCache(
          std::move(relationship));
    }

    cachedStmtPair.first = relatedStatements.get();
    cachedStmtPair.second = ownerResultRelationships.get();

    relationshipStorage->storeInEntityVectorOwnerCache(
        std::move(relatedStatements));
    relationshipStorage->storeInRelationshipVectorOwnerCache(
        std::move(ownerResultRelationships));
  }

  if (givenEntityType == Statement::getEntityTypeStatic()) {
    return cachedStmtPair;
  }

  // filter statements by given entity type and cache respective entities and
  // relationships

  auto filteredStatements = std::make_unique<std::vector<Entity *>>();
  auto filteredResultRelationships =
      std::make_unique<std::vector<Relationship *>>();

  for (int i = 0; i < cachedStmtPair.first->size(); i++) {
    auto *cachedStmt = cachedStmtPair.first->at(i);
    auto *cachedRelationship = cachedStmtPair.second->at(i);
    if (cachedStmt->getEntityType() == givenEntityType) {
      filteredStatements->push_back(cachedStmt);
      filteredResultRelationships->push_back(cachedRelationship);
    }
  }

  cachedPair.first = filteredStatements.get();
  cachedPair.second = filteredResultRelationships.get();

  relationshipStorage->storeInEntityVectorOwnerCache(
      std::move(filteredStatements));
  relationshipStorage->storeInRelationshipVectorOwnerCache(
      std::move(filteredResultRelationships));

  return cachedPair;
}

auto CFGRelationshipEvaluator::
    shouldEvaluateRelationshipsByEntityTypesInReverse(
        const EntityType &leftEntityType, const EntityType &rightEntityType)
        -> bool {
  return false;
  // auto *leftEntityList = getEvaluableEntitiesFromEntityType(leftEntityType);
  // auto *rightEntityList =
  // getEvaluableEntitiesFromEntityType(rightEntityType);

  // int numberOfForwardRelationshipEvaluationsRequired = 0;

  // for (auto *leftEntity : *leftEntityList) {
  //   if (this->relationshipStorage
  //           ->getCachedResultsFromLiteralSynonymCache(
  //               this->getRelationshipType(), leftEntity->getEntityKey(),
  //               rightEntityType) == nullptr) {
  //     numberOfForwardRelationshipEvaluationsRequired++;
  //   }
  // }

  // int numberOfReverseRelationshipEvaluationsRequired = 0;

  // for (auto *rightEntity : *rightEntityList) {
  //   if (this->relationshipStorage
  //           ->getCachedEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
  //               this->getRelationshipType(), leftEntityType,
  //               rightEntity->getEntityKey()) == nullptr) {
  //     numberOfReverseRelationshipEvaluationsRequired++;
  //   }
  // }

  // return numberOfForwardRelationshipEvaluationsRequired >
  //        numberOfReverseRelationshipEvaluationsRequired;
}

auto CFGRelationshipEvaluator::getCachedEntitiesAndRelationships(
    bool isReverse, Entity &sourceEntity,
    const EntityType &destinationEntityType)
    -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> & {
  return isReverse
             ? relationshipStorage->getCachedResultsFromSynonymLiteralCache(
                   this->getRelationshipType(), destinationEntityType,
                   sourceEntity.getEntityKey())
             : relationshipStorage->getCachedResultsFromLiteralSynonymCache(
                   this->getRelationshipType(), sourceEntity.getEntityKey(),
                   destinationEntityType);
}

Relationship *
CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByGivenEntities(
    Entity *leftEntity, Entity *rightEntity) {
  if (!Statement::isStatement(leftEntity) ||
      !Statement::isStatement(rightEntity)) {
    return nullptr;
  }

  if (!isValidEntityInput(leftEntity) || !isValidEntityInput(rightEntity)) {
    return nullptr;
  }

  auto relationshipKey =
      RelationshipKey(&getRelationshipType(), &leftEntity->getEntityKey(),
                      &rightEntity->getEntityKey());

  // checks if this relationship has already been evaluated
  if (relationshipStorage->containedInRelationshipCache(relationshipKey)) {
    return relationshipStorage->getCachedRelationship(relationshipKey);
  }

  auto *leftStatement = dynamic_cast<Statement *>(leftEntity);
  auto *rightStatement = dynamic_cast<Statement *>(rightEntity);

  auto *possibleReverseRelatedRelationships =
      getCachedEntitiesAndRelationships(true, *rightStatement,
                                        leftStatement->getEntityType())
          .second;

  auto *possibleForwardRelatedRelationships =
      getCachedEntitiesAndRelationships(false, *leftStatement,
                                        rightStatement->getEntityType())
          .second;

  std::vector<Relationship *> *relationshipsToCheck;
  if (possibleReverseRelatedRelationships != nullptr &&
      possibleForwardRelatedRelationships != nullptr) {
    relationshipsToCheck = possibleForwardRelatedRelationships->size() <=
                                   possibleReverseRelatedRelationships->size()
                               ? possibleForwardRelatedRelationships
                               : possibleForwardRelatedRelationships;
  } else if (possibleReverseRelatedRelationships != nullptr) {
    relationshipsToCheck = possibleReverseRelatedRelationships;
  } else if (possibleForwardRelatedRelationships != nullptr) {
    relationshipsToCheck = possibleForwardRelatedRelationships;
  } else {
    relationshipsToCheck =
        evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
            rightEntity->getEntityType(), leftEntity, false)
            .second;
  }

  Relationship *resultRelationship = nullptr;

  for (auto *relationship : *relationshipsToCheck) {
    relationshipStorage->storeInRelationshipMapCache(relationship);
    if (relationship->getRightHandEntity()->equals(rightStatement)) {
      resultRelationship = relationship;
    }
  }

  return resultRelationship;
}

auto CFGRelationshipEvaluator::isValidEntityInput(Entity *entity) -> bool {
  return Statement::isStatement(entity);
}

auto CFGRelationshipEvaluator::isValidEntityTypeInput(
    const EntityType &entityType) -> bool {
  return StatementType::isStatementType(entityType);
}

auto CFGRelationshipEvaluator::getEvaluableEntitiesFromEntityType(
    const EntityType &entityType) -> std::vector<Entity *> * {
  if (!StatementType::isStatementType(entityType)) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->entityManager->getEntitiesByType(entityType);
}

auto CFGRelationshipEvaluator::getCFG() -> cfg::CFG * { return this->cfg; }

auto CFGRelationshipEvaluator::getRelationshipStorage()
    -> RelationshipStorage * {
  return this->relationshipStorage;
}

auto CFGRelationshipEvaluator::getEntityManager() -> EntityManager * {
  return this->entityManager;
}
