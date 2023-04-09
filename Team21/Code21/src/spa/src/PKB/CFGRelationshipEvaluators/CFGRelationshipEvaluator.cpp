//
// Created by Aiken Wong on 26/3/23.
//

#include "CFGRelationshipEvaluator.h"

#include "PKB/RelationshipManager.h"

std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>
    CFGRelationshipEvaluator::emptyEntityVectorRelationshipVectorPair =
        std::make_pair(RelationshipManager::getEmptyEntityVector(),
                       RelationshipManager::getEmptyRelationshipVector());

CFGRelationshipEvaluator::CFGRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                                   RelationshipCache *relationshipCache, EntityManager *entityManager)
    : cfg(cfg),
      relationshipStorage(relationshipStorage),
      entityManager(entityManager), relationshipCache(relationshipCache) {}

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

  // sort left entity list by statement number, from highest to lowest, into a
  // new list

  auto *possibleCachedRelationships =
      relationshipCache->getCachedRelationshipsByTypes(
          getRelationshipType(), leftEntityType, rightEntityType);

  if (possibleCachedRelationships != nullptr) {
    return possibleCachedRelationships;
  }

  bool isReverse = shouldEvaluateRelationshipsByEntityTypesInReverse(
      leftEntityList, rightEntityList);

  auto results = std::make_unique<std::vector<Relationship *>>();

  auto *sourceEntityList = isReverse ? rightEntityList : leftEntityList;
  auto sortedList = std::vector<Entity *>();

  if (this->shouldSortForDoubleEnityTypeEvaluation()) {
    sortedList = std::vector<Entity *>(*sourceEntityList);
    // sort by statement number from highest to lowest, as typically (but not always), stmts on
    // top should affect stmts below, this would allow for faster evaluation for
    // affects* under typical circumstances
    std::sort(sortedList.begin(), sortedList.end(), [](Entity *a, Entity *b) {
      return *a->getEntityKey().getOptionalInt() >
             *b->getEntityKey().getOptionalInt();
    });

    sourceEntityList = &sortedList;
  }

  for (auto *entity : *sourceEntityList) {
    auto *partialResults =
        evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
            isReverse ? leftEntityType : rightEntityType, entity, isReverse)
            .second;

    for (auto *partialResult : *partialResults) {
      results->push_back(partialResult);
    }
  }

  auto *resultsPtr = results.get();
  relationshipCache->storeInRelationshipDoubleSynonymCache(
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
      relationshipCache->storeInRelationshipOwnerCache(
          std::move(relationship));
    }

    cachedStmtPair.first = relatedStatements.get();
    cachedStmtPair.second = ownerResultRelationships.get();

    relationshipCache->storeInEntityVectorOwnerCache(
        std::move(relatedStatements));
    relationshipCache->storeInRelationshipVectorOwnerCache(
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

  relationshipCache->storeInEntityVectorOwnerCache(
      std::move(filteredStatements));
  relationshipCache->storeInRelationshipVectorOwnerCache(
      std::move(filteredResultRelationships));

  return cachedPair;
}

auto CFGRelationshipEvaluator::shouldSortForDoubleEnityTypeEvaluation()
    -> bool {
  return false;
}

auto CFGRelationshipEvaluator::getCachedEntitiesAndRelationships(
    bool isReverse, Entity &sourceEntity,
    const EntityType &destinationEntityType)
    -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> & {
  return isReverse
             ? relationshipCache->getCachedResultsFromSynonymLiteralCache(
                   this->getRelationshipType(), destinationEntityType,
                   sourceEntity.getEntityKey())
             : relationshipCache->getCachedResultsFromLiteralSynonymCache(
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
  if (relationshipCache->containedInRelationshipCache(relationshipKey)) {
    return relationshipCache->getCachedRelationship(relationshipKey);
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
    relationshipCache->storeInRelationshipMapCache(relationship);
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

auto CFGRelationshipEvaluator::getRelationshipCache() -> RelationshipCache * {
    return this->relationshipCache;
}


auto CFGRelationshipEvaluator::getEntityManager() -> EntityManager * {
  return this->entityManager;
}
