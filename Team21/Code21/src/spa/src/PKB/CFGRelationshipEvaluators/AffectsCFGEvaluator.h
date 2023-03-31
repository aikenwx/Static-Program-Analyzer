//
// Created by Aiken Wong on 29/3/23.
//

#ifndef SPA_AFFECTSCFGEVALUATOR_H
#define SPA_AFFECTSCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"

class AffectsCFGEvaluator : public CFGRelationshipEvaluator {
 public:
  AffectsCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                      EntityManager* entityManager);

  auto getRelationshipType() const -> const RelationshipType& override;

  auto createNewRelationship(Statement* leftStatement,
                             Statement* rightStatement)
      -> std::shared_ptr<Relationship> override;

  auto getRelatedBlockStatementPairs(
      std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
      bool isReverse)
      -> std::shared_ptr<std::vector<
          std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> override;

  auto isValidEntityInput(Entity* entity) -> bool override;

  auto isValidEntityTypeInput(const EntityType& entityType) -> bool override;
};

#endif  // SPA_AFFECTSCFGEVALUATOR_H
