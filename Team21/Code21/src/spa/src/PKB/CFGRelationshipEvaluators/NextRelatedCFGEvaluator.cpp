//
// Created by Aiken Wong on 6/4/23.
//

#include "NextRelatedCFGEvaluator.h"

NextRelatedCFGEvaluator::NextRelatedCFGEvaluator(cfg::CFG* cfg,
                                   RelationshipStorage* relationshipStorage,
                                   EntityManager* entityManager)
        : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}


auto NextRelatedCFGEvaluator::getRelatedStatements(Statement *statement,
                                                    bool isReverse)
-> std::unique_ptr<std::vector<Entity *>> {
    auto relatedStmtNumbers =
            getRelatedStatementNumbers(statement->getStatementNumber(), isReverse);

    auto relatedStatements = std::make_unique<std::vector<Entity *>>();

    for (auto stmtNumber : *relatedStmtNumbers) {
        relatedStatements->push_back(getEntityManager()->getStmtByNumber(stmtNumber));
    }

    return relatedStatements;
}

