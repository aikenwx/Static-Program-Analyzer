//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_CFGEVALUATABLERELATIONSHIPTYPE_H
#define SPA_CFGEVALUATABLERELATIONSHIPTYPE_H

#include "PKB/CFGRelationshipEvaluators/CFGRelationshipEvaluator.h"
#include "Relationship.h"
#include "PKB/RelationshipCache.h"

class CFGEvaluableRelationshipType : public RelationshipType {
 public:
  static auto isCFGEvaluableRelationship(const RelationshipType& relationshipType) -> bool;

  virtual auto getRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                        RelationshipCache *relationshipCache,
                                        EntityManager *entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> = 0;
};

#endif  // SPA_CFGEVALUATABLERELATIONSHIPTYPE_H
