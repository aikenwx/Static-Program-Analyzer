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

void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType) {
  auto *leftEntityList = getEvaluableEntitiesFromEntityType(leftEntityType);
  auto *rightEntityList = getEvaluableEntitiesFromEntityType(rightEntityType);
  this->entityManager->getEntitiesByType(rightEntityType);

  // if either list is empty, no need to evaluate, relationships cannot exist
  if (leftEntityList->empty() || rightEntityList->empty()) {
    return;
  }
  // if contents of either list is not a statement, no need to evaluate, as they
  // are not CFG evaluable Relationships
  if (!Statement::isStatement(leftEntityList->at(0)) ||
      !Statement::isStatement(rightEntityList->at(0))) {
    return;
  }

  if (!isValidEntityTypeInput(leftEntityType) ||
      !isValidEntityTypeInput(rightEntityType)) {
    return;
  }

  // already evaluated
  if (relationshipStorage->getRelationshipsByTypes(
          getRelationshipType(), leftEntityType, rightEntityType) != nullptr) {
    return;
  }

  auto relationshipDoubleSynonymKey = RelationshipDoubleSynonymKey(
      &getRelationshipType(), &leftEntityType, &rightEntityType);

  relationshipStorage
      ->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
          relationshipDoubleSynonymKey, true);

  bool isReverse = shouldEvaluateRelationshipsByEntityTypesInReverse(
      leftEntityType, rightEntityType);

  for (auto *entity : isReverse ? *rightEntityList : *leftEntityList) {
    evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        isReverse ? leftEntityType : rightEntityType, entity, isReverse);

    initializeCacheGivenEntityAndEntityType(
        isReverse, *entity, isReverse ? leftEntityType : rightEntityType);

    auto *results =
        getEntitiesFromStore(isReverse, *dynamic_cast<Statement *>(entity),
                             isReverse ? leftEntityType : rightEntityType);

    for (auto *result : *results) {
      auto relationshipKey =
          isReverse
              ? RelationshipKey(&getRelationshipType(), &result->getEntityKey(),
                                &entity->getEntityKey())
              : RelationshipKey(&getRelationshipType(), &entity->getEntityKey(),
                                &result->getEntityKey());

      this->relationshipStorage->storeInSpecifiedRelationshipDoubleSynonymStore(
          relationshipStorage->getRelationship(relationshipKey),
          relationshipDoubleSynonymKey, true);
    }
  }
}

void CFGRelationshipEvaluator::
    evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        const EntityType &givenEntityType, Entity *entity, bool isReverse) {
  auto *entityList = this->entityManager->getEntitiesByType(givenEntityType);

  if (!isValidEntityInput(entity)) {
    return;
  }

  if (entityList->empty()) {
    return;
  }

  if (!Statement::isStatement(entityList->at(0)) ||
      !Statement::isStatement(entity)) {
    return;
  }

  if (!isValidEntityInput(entity) || !isValidEntityTypeInput(givenEntityType)) {
    return;
  }

  auto *statement = dynamic_cast<Statement *>(entity);

  // already evaluated
  if (getEntitiesFromStore(isReverse, *statement,
                           Statement::getEntityTypeStatic()) != nullptr) {
    return;
  }

  auto relatedStatements = this->getRelatedStatements(statement, isReverse);

  initializeCacheGivenEntityAndEntityType(isReverse, *statement,
                                          Statement::getEntityTypeStatic());

  for (const auto &relatedStatement : *relatedStatements) {
    auto relationship =
        isReverse ? createNewRelationship(relatedStatement, statement)
                  : createNewRelationship(statement, relatedStatement);
    populateCache(isReverse, relationship);
  }
}

auto CFGRelationshipEvaluator::
    shouldEvaluateRelationshipsByEntityTypesInReverse(
        const EntityType &leftEntityType, const EntityType &rightEntityType)
        -> bool {
  auto *leftEntityList = getEvaluableEntitiesFromEntityType(leftEntityType);
  auto *rightEntityList = getEvaluableEntitiesFromEntityType(rightEntityType);

  int numberOfForwardRelationshipEvaluationsRequired = 0;

  for (auto *leftEntity : *leftEntityList) {
    if (this->relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                this->getRelationshipType(), leftEntity->getEntityKey(),
                rightEntityType) == nullptr) {
      numberOfForwardRelationshipEvaluationsRequired++;
    }
  }

  int numberOfReverseRelationshipEvaluationsRequired = 0;

  for (auto *rightEntity : *rightEntityList) {
    if (this->relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                this->getRelationshipType(), leftEntityType,
                rightEntity->getEntityKey()) == nullptr) {
      numberOfReverseRelationshipEvaluationsRequired++;
    }
  }

  return numberOfForwardRelationshipEvaluationsRequired >
         numberOfReverseRelationshipEvaluationsRequired;
}

auto CFGRelationshipEvaluator::getEntitiesFromStore(
    bool isReverse, Entity &sourceEntity,
    const EntityType &destinationEntityType) -> std::vector<Entity *> * {
  return isReverse
             ? relationshipStorage
                   ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                       this->getRelationshipType(), destinationEntityType,
                       sourceEntity.getEntityKey())
             : relationshipStorage
                   ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                       this->getRelationshipType(), sourceEntity.getEntityKey(),
                       destinationEntityType);
}

void CFGRelationshipEvaluator::initializeCacheGivenEntityAndEntityType(
    bool isReverse, Entity &statement, const EntityType &entityType) {
  if (isReverse) {
    RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
        RelationshipSynonymLiteralKey(&this->getRelationshipType(), &entityType,
                                      &statement.getEntityKey());

    relationshipStorage
        ->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
            relationshipSynonymLiteralKey, true);

  } else {
    RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
        RelationshipLiteralSynonymKey(&this->getRelationshipType(),
                                      &statement.getEntityKey(), &entityType);
    relationshipStorage
        ->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
            relationshipLiteralSynonymKey, true);
  }
}

void CFGRelationshipEvaluator::populateCache(
    bool isReverse, const std::shared_ptr<Relationship> &relationship) {
  if (isReverse) {
    relationshipStorage->tryStoreRelationshipOnlyInRelationshipStore(
        relationship, true);

    relationshipStorage->storeInRelationshipSynonymLiteralStore(
        relationship.get(), true);
  } else {
    relationshipStorage->tryStoreRelationshipOnlyInRelationshipStore(
        relationship, true);

    relationshipStorage->storeInRelationshipLiteralSynonymStore(
        relationship.get(), true);
  }
}

void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByGivenEntities(
    Entity *leftEntity, Entity *rightEntity) {
  if (!Statement::isStatement(leftEntity) ||
      !Statement::isStatement(rightEntity)) {
    return;
  }

  if (!isValidEntityInput(leftEntity) || !isValidEntityInput(rightEntity)) {
    return;
  }

  auto relationshipKey =
      RelationshipKey(&getRelationshipType(), &leftEntity->getEntityKey(),
                      &rightEntity->getEntityKey());

  // checks if this relationship has already been evaluated
  if (relationshipStorage->getRelationship(relationshipKey) != nullptr) {
    return;
  }

  auto *leftStatement = dynamic_cast<Statement *>(leftEntity);
  auto *rightStatement = dynamic_cast<Statement *>(rightEntity);

  if (getEntitiesFromStore(true, *rightStatement,
                           Statement::getEntityTypeStatic()) != nullptr) {
    return;
  }

  if (getEntitiesFromStore(false, *leftStatement,
                           Statement::getEntityTypeStatic()) != nullptr) {
    return;
  }

  evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      rightEntity->getEntityType(), leftEntity, false);
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
