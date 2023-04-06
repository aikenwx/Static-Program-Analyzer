//
// Created by Aiken Wong on 26/3/23.
//

#include "CFGRelationshipEvaluator.h"

#include "PKB/RelationshipManager.h"

CFGRelationshipEvaluator::CFGRelationshipEvaluator(
    cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
    EntityManager *entityManager)
    : cfg(cfg),
      relationshipStorage(relationshipStorage),
      entityManager(entityManager) {}

auto CFGRelationshipEvaluator::generateStatementBlockPair(Statement *statement)
    -> std::shared_ptr<std::pair<cfg::Block *, Statement *>> {
  auto cfgBlock = cfg->GetBlockAt(statement->getStatementNumber());

  auto sourceBlockStatementPairs =
      std::make_shared<std::pair<cfg::Block *, Statement *>>(
          cfgBlock.value().get(), statement);
  return sourceBlockStatementPairs;
}

auto CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntityTypes(
        const EntityType &leftEntityType, const EntityType &rightEntityType) -> std::vector<Relationship *>
    * {
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

  auto results = std::make_shared<std::vector<Relationship *>>();

  for (auto *entity : isReverse ? *rightEntityList : *leftEntityList) {
    auto *partialResults =
        evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
            isReverse ? leftEntityType : rightEntityType, entity, isReverse)
            .second;

    for (auto *partialResult : *partialResults) {
      results->push_back(partialResult);
    }
  }

  relationshipStorage->storeInRelationshipDoubleSynonymCache(
      getRelationshipType(), leftEntityType, rightEntityType, results);

  return results.get();
}

auto
CFGRelationshipEvaluator::
    evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        const EntityType &givenEntityType, Entity *entity, bool isReverse) -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> {
  auto *entityList = this->entityManager->getEntitiesByType(givenEntityType);

  if (entityList->empty()) {
    return std::make_pair(RelationshipManager::getEmptyEntityVector(),
                          RelationshipManager::getEmptyRelationshipVector());
  }

  if (!Statement::isStatement(entityList->at(0)) ||
      !Statement::isStatement(entity)) {
    return std::make_pair(RelationshipManager::getEmptyEntityVector(),
                          RelationshipManager::getEmptyRelationshipVector());
  }

  if (!isValidEntityInput(entity) || !isValidEntityTypeInput(givenEntityType)) {
    return std::make_pair(RelationshipManager::getEmptyEntityVector(),
                          RelationshipManager::getEmptyRelationshipVector());
  }

  auto *statement = dynamic_cast<Statement *>(entity);

  // already evaluated
  auto *possibleCachedStatements =
      getEntitiesFromStore(isReverse, *statement, givenEntityType);
  std::vector<Relationship *> *possibleCachedRelationships = nullptr;

  if (possibleCachedStatements != nullptr) {
    return std::make_pair(
        possibleCachedStatements,
        getCachedRelationships(isReverse, *statement, givenEntityType));
  }

  possibleCachedStatements =
      getEntitiesFromStore(isReverse, *statement, Statement::getEntityTypeStatic());

  if (possibleCachedStatements == nullptr) {
    auto relatedStatements = this->getRelatedStatements(statement, isReverse);

    isReverse
        ? getRelationshipStorage()->storeInRelationshipSynonymLiteralCache(
              getRelationshipType(), Statement::getEntityTypeStatic(),
              entity->getEntityKey(), relatedStatements)
        : getRelationshipStorage()->storeInRelationshipLiteralSynonymCache(
              getRelationshipType(), entity->getEntityKey(),
              Statement::getEntityTypeStatic(), relatedStatements);

    possibleCachedStatements = relatedStatements.get();

    auto ownerResultRelationships =
        std::make_shared<std::vector<Relationship *>>();

    for (auto *relatedStatement : *relatedStatements) {
      auto relationship =
          isReverse ? createNewRelationship(relatedStatement, statement)
                    : createNewRelationship(statement, relatedStatement);

      relationshipStorage->storeInRelationshipOwnerCache(relationship);
      ownerResultRelationships->push_back(relationship.get());
    }

    isReverse
        ? getRelationshipStorage()->storeInRelationshipSynonymLiteralCache(
              getRelationshipType(), Statement::getEntityTypeStatic(),
              entity->getEntityKey(), ownerResultRelationships)
        : getRelationshipStorage()->storeInRelationshipLiteralSynonymCache(
              getRelationshipType(), entity->getEntityKey(),
              Statement::getEntityTypeStatic(), ownerResultRelationships);

    possibleCachedRelationships = ownerResultRelationships.get();
  } else {
    possibleCachedRelationships =
        getCachedRelationships(isReverse, *statement, Statement::getEntityTypeStatic());
  }

  if (givenEntityType == Statement::getEntityTypeStatic()) {
    return std::make_pair(possibleCachedStatements,
                          possibleCachedRelationships);
  }

  // filter statements by given entity type and cache respective entities and
  // relationships

  auto filteredStatements = std::make_shared<std::vector<Entity *>>();
  auto filteredResultRelationships =
      std::make_shared<std::vector<Relationship *>>();

  for (int i = 0; i < possibleCachedStatements->size(); i++) {
    auto *cachedStmt = possibleCachedStatements->at(i);
    auto *cachedRelationship = possibleCachedRelationships->at(i);
    if (cachedStmt->getEntityType() == givenEntityType) {
      filteredStatements->push_back(cachedStmt);
      filteredResultRelationships->push_back(cachedRelationship);
    }
  }

  isReverse
      ? getRelationshipStorage()->storeInRelationshipSynonymLiteralCache(
            getRelationshipType(), givenEntityType,
            entity->getEntityKey(), filteredResultRelationships)
      : getRelationshipStorage()->storeInRelationshipLiteralSynonymCache(
            getRelationshipType(), entity->getEntityKey(),
            givenEntityType, filteredResultRelationships);

  isReverse ? getRelationshipStorage()->storeInRelationshipSynonymLiteralCache(
                  getRelationshipType(), givenEntityType,
                  entity->getEntityKey(), filteredStatements)
            : getRelationshipStorage()->storeInRelationshipLiteralSynonymCache(
                  getRelationshipType(), entity->getEntityKey(),
                  givenEntityType, filteredStatements);

  return std::make_pair(filteredStatements.get(),
                        filteredResultRelationships.get());
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
  //           ->getCachedEntitiesForGivenRelationshipTypeAndRightHandEntityType(
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

auto CFGRelationshipEvaluator::getEntitiesFromStore(
    bool isReverse, Entity &sourceEntity,
    const EntityType &destinationEntityType) -> std::vector<Entity *> * {
  return isReverse
             ? relationshipStorage
                   ->getCachedEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                       this->getRelationshipType(), destinationEntityType,
                       sourceEntity.getEntityKey())
             : relationshipStorage
                   ->getCachedEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                       this->getRelationshipType(), sourceEntity.getEntityKey(),
                       destinationEntityType);
}

auto CFGRelationshipEvaluator::getCachedRelationships(
    bool isReverse, Entity &sourceEntity,
    const EntityType &destinationEntityType) -> std::vector<Relationship *> * {
  return isReverse
             ? relationshipStorage
                   ->getCachedRelationshipsForGivenRelationshipTypeAndLeftHandEntityType(
                       this->getRelationshipType(), destinationEntityType,
                       sourceEntity.getEntityKey())
             : relationshipStorage
                   ->getCachedRelationshipsForGivenRelationshipTypeAndRightHandEntityType(
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

  auto *possibleReverseRelatedRelationships = getCachedRelationships(
      true, *rightStatement, leftStatement->getEntityType());

  auto *possibleForwardRelatedRelationships = getCachedRelationships(
      false, *leftStatement, rightStatement->getEntityType());

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
    if (relationship->getRightHandEntity()->equals(rightStatement)) {
      resultRelationship = relationship;
      break;
    }
  }

  relationshipStorage->storeInRelationshipMapCache(resultRelationship);

  return resultRelationship;
}

// if (getEntitiesFromStore(true, *rightStatement,
//                          Statement::getEntityTypeStatic()) != nullptr) {
//   return;
// }

// if (getEntitiesFromStore(false, *leftStatement,
//                          Statement::getEntityTypeStatic()) != nullptr) {
//   return;
// }

// evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
//     rightEntity->getEntityType(), leftEntity, false);
// }

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
