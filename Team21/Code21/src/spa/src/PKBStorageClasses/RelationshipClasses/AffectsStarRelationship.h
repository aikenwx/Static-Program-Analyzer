//
// Created by Aiken Wong on 27/3/23.
//

#ifndef SPA_AFFECTSSTARRELATIONSHIP_H
#define SPA_AFFECTSSTARRELATIONSHIP_H

#include "CFGEvaluatableRelationshipType.h"
#include "PKB/CFGRelationshipEvaluators/AffectsStarCFGEvaluator.h"
#include "Relationship.h"

class AffectsStarRelationshipType : public CFGEvaluableRelationshipType {
 public:
  auto getRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> override;
};
class AffectsStarRelationship : public Relationship {
 private:
  static const AffectsStarRelationshipType relationshipType;

 public:
  static auto getRelationshipTypeStatic() -> const RelationshipType&;

  AffectsStarRelationship(AssignStatement* affecter, AssignStatement* affectee);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType& override;
};

#endif  // SPA_AFFECTSSTARRELATIONSHIP_H
