//
// Created by Aiken Wong on 26/3/23.
//

#include "CFGRelationshipEvaluator.h"

auto ForwardBlockStatementPairIterator::nextBlockStatementPairs(
    std::pair<cfg::Block *, int> &currentBlockStatementPair)
    -> std::shared_ptr<
        std::vector<std::shared_ptr<std::pair<cfg::Block *, int>>>> {
  auto nextBlockStatementPairs = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block *, int>>>>();
  auto currentBlock = currentBlockStatementPair.first;
  auto currentStatement = currentBlockStatementPair.second;

  int possibleNextStatement = currentStatement + 1;

  if (currentBlock->IsInBlock(possibleNextStatement)) {
    nextBlockStatementPairs->push_back(
        std::make_shared<std::pair<cfg::Block *, int>>(currentBlock,
                                                       possibleNextStatement));
  } else {
    auto neighbours = currentBlock->children();
    for (auto neighbour : neighbours) {
      nextBlockStatementPairs->push_back(
          std::make_shared<std::pair<cfg::Block *, int>>(
              neighbour.lock().get(), neighbour.lock()->start()));
    }
  }

  return nextBlockStatementPairs;
}

auto ReverseBlockStatementPairIterator::nextBlockStatementPairs(
    std::pair<cfg::Block *, int> &currentBlockStatementPair)
    -> std::shared_ptr<
        std::vector<std::shared_ptr<std::pair<cfg::Block *, int>>>> {
  auto nextBlockStatementPairs = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block *, int>>>>();
  auto currentBlock = currentBlockStatementPair.first;
  auto currentStatement = currentBlockStatementPair.second;

  int possibleNextStatement = currentStatement - 1;

  if (currentBlock->IsInBlock(possibleNextStatement)) {
    nextBlockStatementPairs->push_back(
        std::make_shared<std::pair<cfg::Block *, int>>(currentBlock,
                                                       possibleNextStatement));
  } else {
    auto neighbours = currentBlock->parents();
    for (auto neighbour : neighbours) {
      nextBlockStatementPairs->push_back(
          std::make_shared<std::pair<cfg::Block *, int>>(
              neighbour.lock().get(), neighbour.lock()->end()));
    }
  }
}

CFGRelationshipEvaluator::CFGRelationshipEvaluator(
    cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
    EntityManager *entityManager)
    : cfg(cfg),
      relationshipStorage(relationshipStorage),
      entityManager(entityManager) {}

void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntities(
    Entity *leftEntity, Entity *rightEntity) {
  if (!Statement::isStatement(leftEntity) &&
      !Statement::isStatement(rightEntity)) {
    return;
  }

  auto leftStatement = static_cast<Statement *>(leftEntity);

  this->operator()(leftStatement, false);
}

void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType) {
  auto leftEntityList = this->entityManager->getEntitiesByType(leftEntityType);
  auto rightEntityList =
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

  int leftEntityCount = 0;

  for (auto leftEntity : *leftEntityList) {
    if (this->relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                (RelationshipType &)this->getRelationshipType(),
                leftEntity->getEntityKey(), rightEntityType) == nullptr) {
      leftEntityCount++;
    }
  }

  int rightEntityCount = 0;
  for (auto rightEntity : *rightEntityList) {
    if (this->relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                (RelationshipType &)this->getRelationshipType(), leftEntityType,
                rightEntity->getEntityKey()) == nullptr) {
      rightEntityCount++;
    }
  }

  // evaluate from the smaller list, which has likely lower cost evaluation and
  // cache results directly into relationship storage
  if (leftEntityCount < rightEntityCount) {
    for (auto leftEntity : *leftEntityList) {
      auto results =
          this->operator()(static_cast<Statement *>(leftEntity), false);

      for (auto result : *results) {
        auto relationshipKey =
            RelationshipKey(&getRelationshipType(), &leftEntity->getEntityKey(),
                            &result->getEntityKey());

        auto relationship =
            relationshipStorage->getRelationship(relationshipKey);

        this->relationshipStorage->storeInRelationshipDoubleSynonymStore(
            relationship);
      }
    }
  } else {
    for (auto rightEntity : *rightEntityList) {
      auto results =
          this->operator()(static_cast<Statement *>(rightEntity), true);

      for (auto result : *results) {
        auto relationshipKey =
            RelationshipKey(&getRelationshipType(), &result->getEntityKey(),
                            &rightEntity->getEntityKey());

        auto relationship =
            relationshipStorage->getRelationship(relationshipKey);

        this->relationshipStorage->storeInRelationshipDoubleSynonymStore(
            relationship);
      }
    }
  }
}

void CFGRelationshipEvaluator::
    evaluateAndCacheRelationshipsByLeftEntityTypeAndRightEntity(
        const EntityType &leftEntityType, Entity *rightEntity) {
  auto leftEntityList = this->entityManager->getEntitiesByType(leftEntityType);

  // if either list is empty, no need to evaluate, relationships cannot exist
  if (leftEntityList->empty()) {
    return;
  }
  // if entities are not statements, no need to evaluate, as they are not CFG
  // evaluable Relationships
  if (!Statement::isStatement(leftEntityList->at(0)) ||
      !Statement::isStatement(rightEntity)) {
    return;
  }

  this->operator()(static_cast<Statement *>(rightEntity), true);
}

void CFGRelationshipEvaluator::
    evaluateAndCacheRelationshipsByLeftEntityAndRightEntityType(
        Entity *leftEntity, const EntityType &rightEntityType) {
  auto rightEntityList =
      this->entityManager->getEntitiesByType(rightEntityType);

  // if either list is empty, no need to evaluate, relationships cannot exist
  if (rightEntityList->empty()) {
    return;
  }
  // if entities are not statements, no need to evaluate, as they are not CFG
  // evaluable Relationships
  if (!Statement::isStatement(leftEntity) ||
      !Statement::isStatement(rightEntityList->at(0))) {
    return;
  }

  this->operator()(static_cast<Statement *>(leftEntity), false);
}