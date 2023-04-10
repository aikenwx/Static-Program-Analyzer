//
// Created by Aiken Wong on 25/3/23.
//

#include "CFGEvaluatableRelationshipType.h"

#include "PKB/CFGRelationshipEvaluators/CFGRelationshipEvaluator.h"

auto CFGEvaluableRelationshipType::isCFGEvaluableRelationship(const RelationshipType& relationshipType) -> bool {
  return dynamic_cast<const CFGEvaluableRelationshipType*>(&relationshipType) != nullptr;
}
