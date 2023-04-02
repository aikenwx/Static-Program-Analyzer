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

auto NextCFGEvaluator::getRelatedBlockStatementPairs(
    std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
    bool isReverse)
    -> std::shared_ptr<
        std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> {
  auto nextBlockStatementPairs = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>>();

  auto* currentBlock = sourceBlockStatementPair.first;
  auto* currentStatement = sourceBlockStatementPair.second;

  int possibleNextStatementNumber =
      isReverse ? currentStatement->getStatementNumber() - 1
                : currentStatement->getStatementNumber() + 1;

  if (currentBlock->IsInBlock(possibleNextStatementNumber)) {
    auto entityKey = EntityKey(&Statement::getEntityTypeStatic(),
                               possibleNextStatementNumber);

    auto* statement =
        dynamic_cast<Statement*>(getEntityManager()->getEntity(entityKey));

    nextBlockStatementPairs->push_back(
        std::make_shared<std::pair<cfg::Block*, Statement*>>(currentBlock,
                                                             statement));

  } else {
    auto neighbours =
        isReverse ? currentBlock->parents() : currentBlock->children();
    for (const auto& neighbour : neighbours) {
      auto entityKey = EntityKey(&Statement::getEntityTypeStatic(),
                                 isReverse ? neighbour.lock()->end() : neighbour.lock()->start());

      auto* statement =
          dynamic_cast<Statement*>(getEntityManager()->getEntity(entityKey));

      nextBlockStatementPairs->push_back(
          std::make_shared<std::pair<cfg::Block*, Statement*>>(
              neighbour.lock().get(), statement));
    }
  }

  return nextBlockStatementPairs;
}

auto NextCFGEvaluator::createNewRelationship(Statement* leftStatement,
                                             Statement* rightStatement)
    -> std::shared_ptr<Relationship> {
  return std::make_shared<NextRelationship>(leftStatement, rightStatement);
}
