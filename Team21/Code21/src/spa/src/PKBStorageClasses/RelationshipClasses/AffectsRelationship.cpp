//
// Created by Aiken Wong on 27/3/23.
//

#include "AffectsRelationship.h"

const AffectsRelationshipType AffectsRelationship::relationshipType = AffectsRelationshipType();

auto AffectsRelationship::getRelationshipTypeStatic() -> const RelationshipType& {
  return AffectsRelationship::relationshipType;
}

AffectsRelationship::AffectsRelationship(AssignStatement* affecter, AssignStatement* affectee) : Relationship(&AffectsRelationship::relationshipType, affecter, affectee) {
}

auto AffectsRelationship::getRelationshipType() const -> const RelationshipType& {
  return AffectsRelationship::getRelationshipTypeStatic();
}

auto AffectsRelationshipType::getRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                                       RelationshipCache *relationshipCache,
                                                       EntityManager *entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> const {
  return std::make_shared<AffectsCFGEvaluator>(cfg, relationshipStorage, relationshipCache, entityManager);
}
