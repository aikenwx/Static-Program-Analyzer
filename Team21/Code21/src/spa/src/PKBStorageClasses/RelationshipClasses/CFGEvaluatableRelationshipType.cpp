//
// Created by Aiken Wong on 25/3/23.
//

#include "CFGEvaluatableRelationshipType.h"

#include "PKB/CFGRelationshipEvaluators/CFGRelationshipEvaluator.h"

auto CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(const RelationshipType& relationshipType) -> bool {
  return dynamic_cast<const CFGEvaluatableRelationshipType*>(&relationshipType) != nullptr;
}
