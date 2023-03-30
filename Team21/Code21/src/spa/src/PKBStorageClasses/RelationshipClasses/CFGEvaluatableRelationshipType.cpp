//
// Created by Aiken Wong on 25/3/23.
//

#include "CFGEvaluatableRelationshipType.h"

#include "PKB/CFGRelationshipEvaluators/CFGRelationshipEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"


auto CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(const RelationshipType& relationshipType) -> bool {

  return relationshipType == NextRelationship::getRelationshipTypeStatic() || relationshipType == AffectsRelationship::getRelationshipTypeStatic() ||
         relationshipType == AffectsStarRelationship::getRelationshipTypeStatic() || relationshipType == NextStarRelationship::getRelationshipTypeStatic();
  //  return is_unambiguous_public_base_of<CFGEvaluatableRelationshipType>(&relationshipType) != nullptr;
}
