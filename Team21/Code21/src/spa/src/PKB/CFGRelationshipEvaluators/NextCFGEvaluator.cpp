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

auto NextCFGEvaluator::getRelatedStatements(
    Statement* sourceStatement,
    bool isReverse)
    -> std::shared_ptr<std::vector<Entity *>> {
  auto nextStatements = std::make_shared<
      std::vector<Entity*>>();

  auto* currentBlock = getCFG()->GetBlockAt(sourceStatement->getStatementNumber()).value().get();

  int possibleNextStatementNumber =
      isReverse ? sourceStatement->getStatementNumber() - 1
                : sourceStatement->getStatementNumber() + 1;

  if (currentBlock->IsInBlock(possibleNextStatementNumber)) {
    auto entityKey = EntityKey(&Statement::getEntityTypeStatic(),
                               possibleNextStatementNumber);

    auto* statement = dynamic_cast<Statement*>(getEntityManager()->getEntity(entityKey));

    nextStatements->push_back(statement);

  } else {
    auto neighbours = isReverse ? currentBlock->parents() : currentBlock->children();
    for (const auto& neighbour : neighbours) {
      auto entityKey = EntityKey(&Statement::getEntityTypeStatic(),
                                 isReverse ? neighbour.lock()->end() : neighbour.lock()->start());

      auto* statement = dynamic_cast<Statement*>(getEntityManager()->getEntity(entityKey));

      nextStatements->push_back(statement);
    }
  }

  return nextStatements;
}

auto NextCFGEvaluator::createNewRelationship(Entity *leftStatement,
                                             Entity *rightStatement)
    -> std::shared_ptr<Relationship> {
  return std::make_shared<NextRelationship>(dynamic_cast<Statement *>(leftStatement)  , dynamic_cast<Statement *>(rightStatement));
}
