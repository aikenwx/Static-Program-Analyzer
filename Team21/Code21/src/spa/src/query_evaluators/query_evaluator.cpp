#include "query/query_exceptions.h"
#include "query_evaluator.h"
#include "query_evaluators/select_evaluators/select_evaluator.h"
namespace qps {
std::unordered_set<std::string> QueryEvaluator::evaluateQuery(QueryFacade &pkb) {
  auto target = query_.getSelectClause().getSynonym();
  if (query_.getSuchThatClause().empty()) {
    auto select_evaluator = std::make_unique<SelectEvaluator>(query_.getSelectClause());
    auto res = select_evaluator->evaluate(pkb).Extract(target);
    return res;
  } else {
    throw QueryException("Clauses not supported for now.");
  }
}
} // qps
