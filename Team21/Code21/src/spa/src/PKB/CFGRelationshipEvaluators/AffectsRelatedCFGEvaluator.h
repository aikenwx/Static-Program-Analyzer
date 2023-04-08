//
// Created by Aiken Wong on 31/3/23.
//

#ifndef SPA_AFFECTSRELATEDCFGEVALUATOR_H
#define SPA_AFFECTSRELATEDCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"

class AffectsRelatedCFGEvaluator : public CFGRelationshipEvaluator {
 public:
  AffectsRelatedCFGEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                             RelationshipCache *relationshipCache, EntityManager *entityManager);
  auto isValidEntityInput(Entity *entity) -> bool override;

  auto isValidEntityTypeInput(const EntityType &entityType) -> bool override;

  auto getEvaluableEntitiesFromEntityType(const EntityType &entityType)
      -> std::vector<Entity *> * override;

  static auto isAllowedModifier(Entity *entity) -> bool;

 protected:
  auto shouldEvaluateRelationshipsByEntityTypesInReverse(
      std::vector<Entity *> *leftEntityVector,
      std::vector<Entity *> *rightEntityVector) -> bool override;
};

#endif  // SPA_AFFECTSRELATEDCFGEVALUATOR_H
