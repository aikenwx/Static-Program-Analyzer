//
// Created by Aiken Wong on 29/3/23.
//

#ifndef SPA_AFFECTSSTARCFGEVALUATOR_H
#define SPA_AFFECTSSTARCFGEVALUATOR_H

#include "AffectsRelatedCFGEvaluator.h"
class AffectsStarCFGEvaluator : public AffectsRelatedCFGEvaluator {
 public:
  AffectsStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                          EntityManager* entityManager);

  [[nodiscard]] auto getRelationshipType() const -> const RelationshipType& override;

  auto createNewRelationship(Entity *leftStatement,
                             Entity *rightStatement)
      -> std::shared_ptr<Relationship> override;

  auto getRelatedStatements(
          Statement *sourceStatement,
          bool isReverse)
      -> std::shared_ptr<std::vector<Entity *>> override;
};

#endif  // SPA_AFFECTSSTARCFGEVALUATOR_H
