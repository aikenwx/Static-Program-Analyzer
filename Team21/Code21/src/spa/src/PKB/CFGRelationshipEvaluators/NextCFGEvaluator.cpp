//
// Created by Aiken Wong on 25/3/23.
//

#include "NextCFGEvaluator.h"

#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"

NextCFGEvaluator::NextCFGEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                   RelationshipCache *relationshipCache, EntityManager *entityManager)
    : NextRelatedCFGEvaluator(cfg, relationshipStorage, relationshipCache, entityManager) {}

auto NextCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return NextRelationship::getRelationshipTypeStatic();
}

auto NextCFGEvaluator::getRelatedStatementNumbers(int sourceStatementNumber,
                                                  bool isReverse)
    -> std::unique_ptr<std::vector<int>> {
  auto nextStatementsNumbers = std::make_unique<std::vector<int>>();

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
    -> std::unique_ptr<Relationship> {
  return std::make_unique<NextRelationship>(
      dynamic_cast<Statement*>(leftStatement),
      dynamic_cast<Statement*>(rightStatement));
}

auto NextCFGEvaluator::getRelatedStatements(Statement *statement,
                                                   bool isReverse)
-> std::unique_ptr<std::vector<Entity *>> {
    auto relatedStmtNumbers =
            getRelatedStatementNumbers(statement->getStatementNumber(), isReverse);

    auto relatedStatements = std::make_unique<std::vector<Entity *>>();

    for (auto stmtNumber : *relatedStmtNumbers) {
        relatedStatements->push_back(
                getEntityManager()->getStmtByNumber(stmtNumber));
    }

    return relatedStatements;
}