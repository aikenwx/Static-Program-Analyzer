//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_CFGEVALUATABLERELATIONSHIPTYPE_H
#define SPA_CFGEVALUATABLERELATIONSHIPTYPE_H

#include "PKB/CFGRelationshipEvaluators/CFGRelationshipEvaluator.h"
#include "Relationship.h"

class CFGEvaluatableRelationshipType : public RelationshipType {
 public:
  static auto isCFGEvaluableRelationship(const RelationshipType& relationshipType) -> bool;

  virtual auto getRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> = 0;
};

#endif  // SPA_CFGEVALUATABLERELATIONSHIPTYPE_H
