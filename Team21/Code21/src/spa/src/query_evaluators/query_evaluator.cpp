#include "query/query_exceptions.h"
#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "such_that_evaluators/such_that_evaluator_factory.h"
#include "pattern_evaluators/pattern_evaluator_factory.h"

namespace qps {
std::unordered_set<std::string> QueryEvaluator::evaluateQuery(QueryFacade &pkb) {
  auto target = query_.getSelectClause().getSynonym();
  auto select_evaluator = std::make_unique<SelectEvaluator>(query_.getSelectClause());
  if (query_.getSuchThatClause().empty() && query_.getPatternClause().empty()) {
      return select_evaluator->Evaluate(pkb).Extract(target);
  } else if (!query_.getPatternClause().empty() && query_.getSuchThatClause().empty()) {
    //to be refactored later. Can also another else if for joins.
      auto pattern_clause = query_.getPatternClause()[0];
      auto declarations = query_.getDeclarations();
      auto pattern_evaluator = PatternEvaluatorFactory::Create(pattern_clause, declarations);
      auto clause_result = pattern_evaluator->Evaluate(pkb);
      if (clause_result.HasSynonym(target)) {
          return clause_result.Extract(target);
      }
      else if (!clause_result.Empty()) {
          return select_evaluator->Evaluate(pkb).Extract(target);
      }
      else {
          return {};
      }
  } else if (query_.getPatternClause().empty() && !query_.getSuchThatClause().empty()) {
    auto such_that_clause = query_.getSuchThatClause()[0];
    auto declarations = query_.getDeclarations();
    auto such_that_evaluator = SuchThatEvaluatorFactory::Create(such_that_clause, declarations);
    auto clause_result = such_that_evaluator->Evaluate(pkb);
    if (clause_result.HasSynonym(target)) {
      return clause_result.Extract(target);
    } else if (!clause_result.Empty()) {
      return select_evaluator->Evaluate(pkb).Extract(target);
    } else {
      return {};
    }
  }
}
} // qps
