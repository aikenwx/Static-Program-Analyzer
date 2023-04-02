//
// Created by Aiken Wong on 31/3/23.
//

#ifndef SPA_AFFECTSRELATEDCFGEVALUATOR_H
#define SPA_AFFECTSRELATEDCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"

class AffectsRelatedCFGEvaluator : public CFGRelationshipEvaluator {
 public:
  AffectsRelatedCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                             EntityManager* entityManager);
  auto isValidEntityInput(Entity* entity) -> bool override;

  auto isValidEntityTypeInput(const EntityType& entityType) -> bool override;


         auto getEvaluableEntitiesFromEntityType(
      const EntityType& entityType) -> std::vector<Entity*>* override;
};

#endif  // SPA_AFFECTSRELATEDCFGEVALUATOR_H
