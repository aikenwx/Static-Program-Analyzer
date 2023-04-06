//
// Created by Aiken Wong on 25/3/23.
//

#include "NextCFGEvaluator.h"

#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"

NextCFGEvaluator::NextCFGEvaluator(cfg::CFG* cfg,
                                   RelationshipStorage* relationshipStorage,
                                   EntityManager* entityManager)
    : NextRelatedCFGEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return NextRelationship::getRelationshipTypeStatic();
}

auto NextCFGEvaluator::getRelatedStatementNumbers(int sourceStatementNumber,
                                                  bool isReverse)
    -> std::shared_ptr<std::vector<int>> {
  auto nextStatementsNumbers = std::make_shared<std::vector<int>>();

  auto* currentBlock =
      getCFG()->GetBlockAt(sourceStatementNumber).value().get();

  int possibleNextStatementNumber =
      isReverse ? sourceStatementNumber - 1 : sourceStatementNumber + 1;

  if (currentBlock->IsInBlock(possibleNextStatementNumber)) {
    nextStatementsNumbers->push_back(possibleNextStatementNumber);

  } else {
    auto neighbours =
        isReverse ? currentBlock->parents() : currentBlock->children();
    for (const auto& neighbour : neighbours) {
      nextStatementsNumbers->push_back(isReverse ? neighbour.lock()->end()
                                                 : neighbour.lock()->start());
    }
  }

  return nextStatementsNumbers;
}

auto NextCFGEvaluator::createNewRelationship(Entity* leftStatement,
                                             Entity* rightStatement)
    -> std::shared_ptr<Relationship> {
  return std::make_shared<NextRelationship>(
      dynamic_cast<Statement*>(leftStatement),
      dynamic_cast<Statement*>(rightStatement));
}
