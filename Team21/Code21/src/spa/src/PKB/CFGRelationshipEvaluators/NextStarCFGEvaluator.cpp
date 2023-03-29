//
// Created by Aiken Wong on 25/3/23.
//

#include "NextStarCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"

NextStarCFGEvaluator::NextStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}


auto NextStarCFGEvaluator::getRelationshipType() const -> const RelationshipType & {
    return NextStarRelationship::getRelationshipTypeStatic();
}

//auto createNewRelationship(Statement* leftStatement,
//                           Statement* rightStatement)
//-> std::shared_ptr<Relationship> override;
//
//auto isRelated(std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
//               Statement& destinationStatement, bool isReverse)
//-> bool override;
//auto getRelatedStatements(
//        std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
//        bool isReverse)
//-> std::shared_ptr<std::vector<
//        std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> override;