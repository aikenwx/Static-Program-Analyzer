//
// Created by Aiken Wong on 27/3/23.
//

#include "AffectsStarRelationship.h"

const AffectsStarRelationshipType AffectsStarRelationship::relationshipType =
    AffectsStarRelationshipType();

auto AffectsStarRelationship::getRelationshipTypeStatic()
    -> const RelationshipType& {
  return AffectsStarRelationship::relationshipType;
}

AffectsStarRelationship::AffectsStarRelationship(AssignStatement* affecter,
                                                 AssignStatement* affectee)
    : Relationship(&AffectsStarRelationship::relationshipType, affecter,
                   affectee) {}

auto AffectsStarRelationship::getRelationshipType() const
    -> const RelationshipType& {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}

auto AffectsStarRelationshipType::getRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                                           RelationshipCache *relationshipCache,
                                                           EntityManager *entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> const {
  return std::make_shared<AffectsStarCFGEvaluator>(cfg, relationshipStorage, relationshipCache, entityManager);
}
