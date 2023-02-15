#pragma once

#include "query/such_that_clause.h"
#include "such_that_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class FollowsStarEvaluator : public SuchThatEvaluator {
    public:
        FollowsStarEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
            : SuchThatEvaluator(std::move(clause), std::move(declarations)) {}
        std::vector<::Relationship*> CallPkb(QueryFacade& pkb, EntityType left, EntityType right) override;
        std::vector<EntityType> GetLeftHandTypes(Ref& left_arg) override;
        std::vector<EntityType> GetRightHandTypes(Ref& right_arg) override;
};

} // qps