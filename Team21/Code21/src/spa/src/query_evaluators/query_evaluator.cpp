#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "pattern_evaluators/pattern_evaluator_factory.h"
#include "with_evaluators/with_evaluator_factory.h"
#include "join/constraints_solver.h"
#include "tables/table_helpers.h"
#include "query_evaluators/such_that_evaluators/such_that_evaluator.h"
#include "clause_ranking/clause_ranker.h"

namespace qps {
QueryEvaluator::QueryEvaluator(Query query) : query_(std::move(query)) {}

void QueryEvaluator::CreateClauseEvaluators() {
  ClauseRanker ranker{query_.getDeclarations()};
  ranker.SortQuery(query_);
  const auto &declarations = query_.getDeclarations();
  for (const auto &clause : query_.getWithClause()) {
    clause_evaluators_.push_back(WithEvaluatorFactory::Create(clause, declarations));
  }
  for (const auto &clause : query_.getPatternClause()) {
    clause_evaluators_.push_back(PatternEvaluatorFactory::Create(clause, declarations));
  }
  for (const auto &clause : query_.getSuchThatClause()) {
    clause_evaluators_.push_back(std::make_unique<SuchThatEvaluator>(clause, declarations));
  }
}

void QueryEvaluator::EvaluateSelect(QueryFacade &pkb, ClauseResult clause_result, std::list<std::string> &results) {
  SelectEvaluator select_evaluator{query_.getSelectClause(), query_.getDeclarations()};
  select_evaluator.Evaluate(pkb, std::move(clause_result), results);
}

void QueryEvaluator::EvaluateQuery(QueryFacade &pkb, std::list<std::string> &results) {
  CreateClauseEvaluators();
  auto solver = ConstraintsSolver(pkb);
  auto result = solver.solve(clause_evaluators_);
  EvaluateSelect(pkb, std::move(result), results);
}
}  // namespace qps
