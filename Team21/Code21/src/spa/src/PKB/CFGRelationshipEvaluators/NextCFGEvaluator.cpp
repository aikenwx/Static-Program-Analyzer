//
// Created by Aiken Wong on 25/3/23.
//

#include "NextCFGEvaluator.h"

#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"

NextCFGEvaluator::NextCFGEvaluator(cfg::CFG* cfg,
                                   RelationshipStorage* relationshipStorage,
                                   EntityManager* entityManager)
    : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return NextRelationship::getRelationshipTypeStatic();
}

auto NextCFGEvaluator::operator()(Statement* statement, bool isReverse)
    -> std::vector<Entity*>* {
  std::vector<Entity*>* cachedResult;
  // check store if evaluation has been done before
  if (isReverse) {
    cachedResult =
        relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
                NextRelationship::getRelationshipTypeStatic(),
                Statement::getEntityTypeStatic(), statement->getEntityKey());

  } else {
    cachedResult =
        relationshipStorage
            ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                NextRelationship::getRelationshipTypeStatic(),
                statement->getEntityKey(), Statement::getEntityTypeStatic());
  }

  if (cachedResult != nullptr) {
    return cachedResult;
  }

  std::shared_ptr<BlockStatementPairIterator> blockStatementPairIterator;

  if (isReverse) {
    blockStatementPairIterator =
        std::make_shared<ReverseBlockStatementPairIterator>();
  } else {
    blockStatementPairIterator =
        std::make_shared<ForwardBlockStatementPairIterator>();
  }

  auto startBlock = cfg->GetBlockAt(statement->getStatementNumber());

  if (!startBlock) {
    return new std::vector<Entity*>();
  }

  auto statementNumber = statement->getStatementNumber();
  auto startBlockStatementPair =
      std::make_pair(startBlock.value().get(), statementNumber);

  auto nextBlockStatementPairs =
      blockStatementPairIterator->nextBlockStatementPairs(
          startBlockStatementPair);

  for (auto nextBlockStatementPair : *nextBlockStatementPairs) {
    auto nextStatementNumber = nextBlockStatementPair->second;

    auto entityKey =
        EntityKey(&Statement::getEntityTypeStatic(), nextStatementNumber);
    auto nextStatement = (Statement*)entityManager->getEntity(entityKey);

    std::shared_ptr<Relationship> relationship;
    if (isReverse) {
      relationship =
          std::make_shared<NextRelationship>(nextStatement, statement);

      relationshipStorage->storeRelationshipOnlyInRelationshipStore(
          relationship);

      relationshipStorage->storeInRelationshipSynonymLiteralStore(
          relationship.get());

    } else {
      relationship =
          std::make_shared<NextRelationship>(statement, nextStatement);

      relationshipStorage->storeRelationshipOnlyInRelationshipStore(

          relationship);

      relationshipStorage->storeInRelationshipSynonymLiteralStore(
          relationship.get());
    }
  }

  if (isReverse) {
    return relationshipStorage
        ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
            NextRelationship::getRelationshipTypeStatic(),
            Statement::getEntityTypeStatic(), statement->getEntityKey());
  } else {
    return relationshipStorage
        ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
            NextRelationship::getRelationshipTypeStatic(),
            statement->getEntityKey(), Statement::getEntityTypeStatic());
  }
}