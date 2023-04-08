//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTSTARCFGEVALUATOR_H
#define SPA_NEXTSTARCFGEVALUATOR_H

#include "NextRelatedCFGEvaluator.h"
class NextStarCFGEvaluator : public NextRelatedCFGEvaluator {
 public:
  NextStarCFGEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                       EntityManager *entityManager);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType & override;

  auto createNewRelationship(Entity *leftStatement, Entity *rightStatement)
      -> std::unique_ptr<Relationship> override;

  auto getRelatedStatements(Statement *statement, bool isReverse)
      -> std::unique_ptr<std::vector<Entity *>> override;

  auto shouldSortForDoubleEnityTypeEvaluation() -> bool override;
};

#endif  // SPA_NEXTSTARCFGEVALUATOR_H
