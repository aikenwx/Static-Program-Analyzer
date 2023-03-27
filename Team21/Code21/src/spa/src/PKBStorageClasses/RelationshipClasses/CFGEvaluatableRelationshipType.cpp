//
// Created by Aiken Wong on 25/3/23.
//

#include "CFGEvaluatableRelationshipType.h"

auto CFGEvaluatableRelationshipType::isCFGEvaluableRelationship(const RelationshipType &relationshipType) -> bool {
    return typeid(CFGEvaluatableRelationshipType) == typeid(relationshipType);
}