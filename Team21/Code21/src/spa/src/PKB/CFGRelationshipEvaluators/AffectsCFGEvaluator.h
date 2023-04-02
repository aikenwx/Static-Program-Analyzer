//
// Created by Aiken Wong on 29/3/23.
//

#ifndef SPA_AFFECTSCFGEVALUATOR_H
#define SPA_AFFECTSCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"
#include "AffectsRelatedCFGEvaluator.h"

class AffectsCFGEvaluator : public AffectsRelatedCFGEvaluator {
 public:
  AffectsCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                      EntityManager* entityManager);

  [[nodiscard]] auto getRelationshipType() const -> const RelationshipType& override;

  auto createNewRelationship(Statement* leftStatement,
                             Statement* rightStatement)
      -> std::shared_ptr<Relationship> override;

  auto getRelatedStatements(
          Statement *sourceStatement,
          bool isReverse)
      -> std::shared_ptr<std::vector<Statement *>> override;
};

#endif  // SPA_AFFECTSCFGEVALUATOR_H
