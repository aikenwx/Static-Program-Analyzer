//
// Created by Aiken Wong on 27/3/23.
//

#ifndef SPA_AFFECTSRELATIONSHIP_H
#define SPA_AFFECTSRELATIONSHIP_H

#include "CFGEvaluatableRelationshipType.h"
#include "PKB/CFGRelationshipEvaluators/AffectsCFGEvaluator.h"
#include "Relationship.h"

class AffectsRelationshipType : public CFGEvaluableRelationshipType {
 public:
  auto getRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                RelationshipCache *relationshipCache,
                                EntityManager *entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> const override;
};

class AffectsRelationship : public Relationship {
 private:
  static const AffectsRelationshipType relationshipType;

 public:
  static auto getRelationshipTypeStatic() -> const RelationshipType&;

  AffectsRelationship(AssignStatement* affecter, AssignStatement* affectee);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType& override;
};

#endif  // SPA_AFFECTSRELATIONSHIP_H
