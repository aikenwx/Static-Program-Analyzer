//
// Created by Aiken Wong on 6/4/23.
//

#ifndef SPA_NEXTRELATEDCFGEVALUATOR_H
#define SPA_NEXTRELATEDCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"


class NextRelatedCFGEvaluator: public CFGRelationshipEvaluator {

public:

    NextRelatedCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
            EntityManager* entityManager);

    virtual auto getRelatedStatementNumbers(int sourceStatementNumber, bool isReverse)
    -> std::unique_ptr<std::vector<int>> = 0;

    auto getRelatedStatements(Statement* statement, bool isReverse) -> std::unique_ptr<std::vector<Entity *>> override;
};


#endif //SPA_NEXTRELATEDCFGEVALUATOR_H
