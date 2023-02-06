#include "query/query_exceptions.h"
#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "such_that_evaluators/such_that_evaluator_factory.h"

namespace qps {
std::unordered_set<std::string> QueryEvaluator::evaluateQuery(QueryFacade &pkb) {
  auto target = query_.getSelectClause().getSynonym();
  if (!query_.getPatternClause().empty()) {
    throw QueryException("Pattern clause not supported for now.");
  }

  auto select_evaluator = std::make_unique<SelectEvaluator>(query_.getSelectClause());
  if (query_.getSuchThatClause().empty()) {
    return select_evaluator->evaluate(pkb).Extract(target);
  } else {
    auto such_that_clause = query_.getSuchThatClause()[0];
    auto declarations = query_.getDeclarations();
    auto such_that_evaluator = SuchThatEvaluatorFactory::Create(such_that_clause, declarations);
    auto clause_result = such_that_evaluator->evaluate(pkb);
    if (clause_result.HasSynonym(target)) {
      return clause_result.Extract(target);
    } else if (!clause_result.Empty()) {
      return select_evaluator->evaluate(pkb).Extract(target);
    } else {
      return {};
    }
  }
}
} // qps
