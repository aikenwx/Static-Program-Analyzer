#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

    class AssignEvaluator : public PatternEvaluator {
    public:
        AssignEvaluator(PatternClause clause, std::vector<Declaration> declarations)
            : PatternEvaluator(std::move(clause), std::move(declarations)) {}
        std::vector<Entity> CallPkb(QueryFacade& pkb) override;

        int AssignEvaluator::postfixHelper(char a);

        std::string AssignEvaluator::makePostfix(std::string str);

        std::vector<Entity> AssignEvaluator::checkExpressionContained(std::vector<AssignStatement> pkb_res, std::string postfix, bool is_partial);
    };

} // qps
