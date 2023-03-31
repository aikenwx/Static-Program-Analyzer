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

  auto block = cfgBlock.value().get();

  auto sourceBlockStatementPairs =
      std::make_shared<std::pair<cfg::Block *, Statement *>>(
          cfgBlock.value().get(), statement);
  return sourceBlockStatementPairs;
}

void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType) {
  auto leftEntityList = this->entityManager->getEntitiesByType(leftEntityType);
  auto rightEntityList =
      this->entityManager->getEntitiesByType(rightEntityType);

  if (relationshipStorage->getRelationshipsByTypes(
          getRelationshipType(), leftEntityType, rightEntityType) != nullptr) {
    return;
  }

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

  relationshipStorage->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
      relationshipDoubleSynonymKey);

  bool isReverse = shouldEvaluateRelationshipsByEntityTypesInReverse(
      leftEntityType, rightEntityType);

  for (auto entity : isReverse ? *rightEntityList : *leftEntityList) {
    if (!isValidEntityInput(entity)) {
      continue;
    }

    auto results = this->getRelatedBlockStatementPairs(
        *generateStatementBlockPair(static_cast<Statement *>(entity)),
        isReverse);

    auto statement = static_cast<Statement *>(entity);
    initializeCache(isReverse, *statement);

    for (auto result : *results) {
      auto relationshipKey =
          RelationshipKey(&getRelationshipType(), &entity->getEntityKey(),
                          &result->second->getEntityKey());

      auto relationship =
          isReverse ? createNewRelationship(result->second, statement)
                    : createNewRelationship(statement, result->second);

      this->populateCache(isReverse, relationship);

      this->relationshipStorage->storeInRelationshipDoubleSynonymStore(
          relationshipStorage->getRelationship(relationshipKey));
    }
  }
}

void CFGRelationshipEvaluator::
    evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        const EntityType &givenEntityType, Entity *entity, bool isReverse) {
  auto entityList = this->entityManager->getEntitiesByType(givenEntityType);

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

  auto statement = static_cast<Statement *>(entity);

  // already evaluated
  if (getEntitiesFromCache(isReverse, *statement) != nullptr) {
    return;
  }

  auto relatedStatements = this->getRelatedBlockStatementPairs(
      *generateStatementBlockPair(statement), isReverse);

  initializeCache(isReverse, *statement);

  for (auto relatedStatement : *relatedStatements) {
    auto relationship =
        isReverse ? createNewRelationship(relatedStatement->second, statement)
                  : createNewRelationship(statement, relatedStatement->second);
    populateCache(isReverse, relationship);
  }
}

auto CFGRelationshipEvaluator::
    shouldEvaluateRelationshipsByEntityTypesInReverse(
        const EntityType &leftEntityType, const EntityType &rightEntityType)
        -> bool {
  auto leftEntityList = getEvaluatableEntitiesFromEntityType(leftEntityType);
  auto rightEntityList = getEvaluatableEntitiesFromEntityType(rightEntityType);

  int numberOfForwardRelationshipEvaluationsRequired = 0;

  for (auto leftEntity : *leftEntityList) {
    if (this->relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                (RelationshipType &)this->getRelationshipType(),
                leftEntity->getEntityKey(), rightEntityType) == nullptr) {
      numberOfForwardRelationshipEvaluationsRequired++;
    }
  }

  int numberOfReverseRelationshipEvaluationsRequired = 0;

  for (auto rightEntity : *rightEntityList) {
    if (this->relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                (RelationshipType &)this->getRelationshipType(), leftEntityType,
                rightEntity->getEntityKey()) == nullptr) {
      numberOfReverseRelationshipEvaluationsRequired++;
    }
  }

  return numberOfForwardRelationshipEvaluationsRequired >
         numberOfReverseRelationshipEvaluationsRequired;
}

auto CFGRelationshipEvaluator::getEntitiesFromCache(bool isReverse,
                                                    Statement &statement)
    -> std::vector<Entity *> * {
  return isReverse
             ? relationshipStorage
                   ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                       this->getRelationshipType(),
                       Statement::getEntityTypeStatic(),
                       statement.getEntityKey())
             : relationshipStorage
                   ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                       this->getRelationshipType(), statement.getEntityKey(),
                       Statement::getEntityTypeStatic());
}

void CFGRelationshipEvaluator::initializeCache(bool isReverse,
                                               Statement &statement) {
  if (isReverse) {
    RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
        RelationshipSynonymLiteralKey(&this->getRelationshipType(),
                                      &Statement::getEntityTypeStatic(),
                                      &statement.getEntityKey());

    relationshipStorage
        ->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
            relationshipSynonymLiteralKey);
  } else {
    RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
        RelationshipLiteralSynonymKey(&this->getRelationshipType(),
                                      &statement.getEntityKey(),
                                      &Statement::getEntityTypeStatic());

    relationshipStorage
        ->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
            relationshipLiteralSynonymKey);
  }
}

void CFGRelationshipEvaluator::populateCache(
    bool isReverse, std::shared_ptr<Relationship> relationship) {
  if (isReverse) {
    relationshipStorage->tryStoreRelationshipOnlyInRelationshipStore(relationship);

    relationshipStorage->storeInRelationshipSynonymLiteralStore(
        relationship.get());
  } else {
    relationshipStorage->tryStoreRelationshipOnlyInRelationshipStore(relationship);

    relationshipStorage->storeInRelationshipLiteralSynonymStore(
        relationship.get());
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

  auto leftStatement = static_cast<Statement *>(leftEntity);
  auto rightStatement = static_cast<Statement *>(rightEntity);

  if (getEntitiesFromCache(true, *rightStatement) != nullptr) {
    return;
  }

  if (getEntitiesFromCache(false, *leftStatement) != nullptr) {
    return;
  }

  evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      rightEntity->getEntityType(), leftEntity, false);
}

auto CFGRelationshipEvaluator::isValidEntityInput(Entity *entity) -> bool {
  if (!Statement::isStatement(entity)) {
    return false;
  }
  return true;
}

auto CFGRelationshipEvaluator::isValidEntityTypeInput(
    const EntityType &entityType) -> bool {
  if (!StatementType::isStatementType(entityType)) {
    return false;
  }
  return true;
}

auto CFGRelationshipEvaluator::getEvaluatableEntitiesFromEntityType(
    const EntityType &entityType) -> std::vector<Entity *> * {
  if (!StatementType::isStatementType(entityType)) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->entityManager->getEntitiesByType(entityType);
}