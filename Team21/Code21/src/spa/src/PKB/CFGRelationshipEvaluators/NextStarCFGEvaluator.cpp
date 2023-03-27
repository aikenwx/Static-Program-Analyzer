//
// Created by Aiken Wong on 25/3/23.
//

#include "NextStarCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"

NextStarCFGEvaluator::NextStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}


auto NextStarCFGEvaluator::getRelationshipType() const -> const RelationshipType & {
    return NextStarRelationship::getRelationshipTypeStatic();
}

auto NextStarCFGEvaluator::operator()(Statement* statement, bool isReverse) -> std::vector<Entity*>* {
    return new std::vector<Entity*>;
}