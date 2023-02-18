#include "pattern_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"

namespace qps {

    PatternEvaluator::PatternEvaluator(PatternClause clause, std::vector<Declaration> declarations)
        : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

    ClauseResult PatternEvaluator::Evaluate(QueryFacade& pkb) {
        std::vector<Entity> filtered_statements;
        auto res = CallPkb(pkb);
        for (auto statements : res) {
            filtered_statements.push_back(statements);
        }
        return qps::PatternEvaluator::ConstructResult(filtered_statements);
    }

    ClauseResult PatternEvaluator::ConstructResult(const std::vector<Entity>& statements) {
        ClauseResult result;
        Ref ref = clause_.getArg1();
        if (Synonym* syn = std::get_if<Synonym>(&ref)) {
            result.AddSynonym(*syn, 0);
        }

        for (auto ans : statements) {
            auto res = { ans.getEntityValue() };
            result.AddResult({ res });
        }
        return result;
    }
} // qps
