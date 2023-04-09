//
// Created by Aiken Wong on 26/3/23.
//

#include "CFGRelationshipEvaluator.h"

#include <stack>

#include "PKB/RelationshipManager.h"

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
CacheResult CFGRelationshipEvaluator::emptyCacheResult = CacheResult(RelationshipManager::getEmptyEntityVector(),
                                                                     RelationshipManager::getEmptyRelationshipVector(), false);

CFGRelationshipEvaluator::CFGRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                                   RelationshipCache *relationshipCache, EntityManager *entityManager)
    : cfg(cfg),
      relationshipStorage(relationshipStorage),
      entityManager(entityManager),
      relationshipCache(relationshipCache) {}

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
    // affects* and next* under typical circumstances
    std::sort(sortedList.begin(), sortedList.end(), [](Entity *currEntity, Entity *nextEntity) {
      return *currEntity->getEntityKey().getOptionalInt() >
             *nextEntity->getEntityKey().getOptionalInt();
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
        -> CacheResult & {
  auto *entityList = this->entityManager->getEntitiesByType(givenEntityType);

  if (entityList->empty()) {
    return emptyCacheResult;
  }

  if (!Statement::isStatement(entityList->at(0)) ||
      !Statement::isStatement(entity)) {
    return emptyCacheResult;
  }

  if (!isValidEntityInput(entity) || !isValidEntityTypeInput(givenEntityType)) {
    return emptyCacheResult;
  }

  auto *statement = dynamic_cast<Statement *>(entity);

  // already evaluated

  auto &cacheResult =
      getCachedEntitiesAndRelationships(isReverse, *statement, givenEntityType);

  if (cacheResult.first != nullptr) {
    return cacheResult;
  }

  auto &cachedStmtResult = getCachedEntitiesAndRelationships(
      isReverse, *statement, Statement::getEntityTypeStatic());

  if (cachedStmtResult.first == nullptr) {
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

    cachedStmtResult.first = relatedStatements.get();
    cachedStmtResult.second = ownerResultRelationships.get();

    relationshipCache->storeInEntityVectorOwnerCache(
        std::move(relatedStatements));
    relationshipCache->storeInRelationshipVectorOwnerCache(
        std::move(ownerResultRelationships));
  }

  if (givenEntityType == Statement::getEntityTypeStatic()) {
    return cachedStmtResult;
  }

  // filter statements by given entity type and cache respective entities and
  // relationships

  auto filteredStatements = std::make_unique<std::vector<Entity *>>();
  auto filteredResultRelationships =
      std::make_unique<std::vector<Relationship *>>();

  for (int i = 0; i < cachedStmtResult.first->size(); i++) {
    auto *cachedStmt = cachedStmtResult.first->at(i);
    auto *cachedRelationship = cachedStmtResult.second->at(i);
    if (cachedStmt->getEntityType() == givenEntityType) {
      filteredStatements->push_back(cachedStmt);
      filteredResultRelationships->push_back(cachedRelationship);
    }
  }

  cacheResult.first = filteredStatements.get();
  cacheResult.second = filteredResultRelationships.get();

  relationshipCache->storeInEntityVectorOwnerCache(
      std::move(filteredStatements));
  relationshipCache->storeInRelationshipVectorOwnerCache(
      std::move(filteredResultRelationships));

  return cacheResult;
}

auto CFGRelationshipEvaluator::shouldSortForDoubleEnityTypeEvaluation()
    -> bool {
  return false;
}

auto CFGRelationshipEvaluator::getCachedEntitiesAndRelationships(
    bool isReverse, Entity &sourceEntity,
    const EntityType &destinationEntityType)
    -> CacheResult & {
  return isReverse
             ? relationshipCache->getCachedResultsFromSynonymLiteralCache(
                   this->getRelationshipType(), destinationEntityType,
                   sourceEntity.getEntityKey())
             : relationshipCache->getCachedResultsFromLiteralSynonymCache(
                   this->getRelationshipType(), sourceEntity.getEntityKey(),
                   destinationEntityType);
}

auto
CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByGivenEntities(
    Entity *leftEntity, Entity *rightEntity) -> Relationship * {
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

  auto *possibleCachedRelationship = relationshipCache->getCachedRelationship(relationshipKey);

  if (possibleCachedRelationship != nullptr) {
    return possibleCachedRelationship;
  }

  auto *leftStatement = dynamic_cast<Statement *>(leftEntity);
  auto *rightStatement = dynamic_cast<Statement *>(rightEntity);

  auto *cachedResultsForReverseEvaluation = &getCachedEntitiesAndRelationships(true, *rightStatement,
                                                                               leftStatement->getEntityType());

  auto *cachedResultsForForwardEvaluation = &getCachedEntitiesAndRelationships(false, *leftStatement,
                                                                               rightStatement->getEntityType());

  if (cachedResultsForReverseEvaluation->areRelationshipsIndividuallyCached || cachedResultsForReverseEvaluation->areRelationshipsIndividuallyCached) {
    return nullptr;
  }

  CacheResult *cachedResultToUpdate = nullptr;
  if (cachedResultsForReverseEvaluation->second != nullptr &&
      cachedResultsForForwardEvaluation->second != nullptr) {
    cachedResultToUpdate = cachedResultsForForwardEvaluation->second->size() <=
                           cachedResultsForReverseEvaluation->second->size()
                               ? cachedResultsForForwardEvaluation
                               : cachedResultsForForwardEvaluation;
  } else if (cachedResultsForReverseEvaluation->second != nullptr) {
    cachedResultToUpdate = cachedResultsForReverseEvaluation;
  } else if (cachedResultsForForwardEvaluation->second != nullptr) {
    cachedResultToUpdate = cachedResultsForForwardEvaluation;
  } else {
    cachedResultToUpdate =
        &evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
            rightEntity->getEntityType(), leftEntity, false);
  }

  cachedResultToUpdate->areRelationshipsIndividuallyCached = true;

  Relationship *resultRelationship = nullptr;

  for (auto *relationship : *cachedResultToUpdate->second) {
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

auto CFGRelationshipEvaluator::solveTransitiveRelationship(Statement *sourceStatement,
                                                           bool isReverse, CFGRelationshipEvaluator &baseEvaluator)
    -> std::unique_ptr<std::vector<Entity *>> {
  auto visitedStatementNumbers =
      std::vector<bool>(getEntityManager()->getNumberOfStatements() + 1, false);

  auto results = std::make_unique<std::vector<Entity *>>();

  // create stack of statements to visit
  auto statementNumbersToVisit = std::stack<int>();

  // push all statements from results into stack

  auto *directRelations =
      baseEvaluator
          .getCachedEntitiesAndRelationships(isReverse, *sourceStatement,
                                             Statement::getEntityTypeStatic())
          .first;

  if (directRelations == nullptr) {
    directRelations =
        baseEvaluator
            .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                Statement::getEntityTypeStatic(), sourceStatement, isReverse)
            .first;
  }

  for (auto *result : *directRelations) {
    statementNumbersToVisit.push(*result->getEntityKey().getOptionalInt());
  }

  while (!statementNumbersToVisit.empty()) {
    auto nextToVisit = statementNumbersToVisit.top();
    statementNumbersToVisit.pop();

    if (visitedStatementNumbers.at(nextToVisit)) {
      continue;
    }

    visitedStatementNumbers.at(nextToVisit) = true;
    auto *nextToVisitStmt = getEntityManager()->getStatementByNumber(nextToVisit);
    results->push_back(nextToVisitStmt);

    auto *possibleCachedResults =
        this->getCachedEntitiesAndRelationships(
                isReverse, *nextToVisitStmt, Statement::getEntityTypeStatic())
            .first;

    if (possibleCachedResults != nullptr) {
      for (const auto &result : *possibleCachedResults) {
        int stmtNumber = *result->getEntityKey().getOptionalInt();

        if (!visitedStatementNumbers.at(stmtNumber)) {
          visitedStatementNumbers.at(stmtNumber) = true;
          results->push_back(result);
        }
      }
      continue;
    }

    auto *neighbours =
        baseEvaluator
            .getCachedEntitiesAndRelationships(isReverse, *nextToVisitStmt,
                                               Statement::getEntityTypeStatic())
            .first;

    if (neighbours == nullptr) {
      neighbours =
          baseEvaluator
              .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                  Statement::getEntityTypeStatic(), nextToVisitStmt, isReverse)
              .first;
    }
    for (auto *neighbour : *neighbours) {
      statementNumbersToVisit.push(*neighbour->getEntityKey().getOptionalInt());
    }
  }

  return results;
}
