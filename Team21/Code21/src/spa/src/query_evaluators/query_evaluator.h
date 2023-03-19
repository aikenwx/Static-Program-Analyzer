#pragma once

#include <unordered_set>
#include <list>

#include "query/query.h"
#include "PKB/QueryFacade.h"
#include "clause_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "such_that_evaluators/such_that_evaluator_factory.h"
#include "pattern_evaluators/pattern_evaluator_factory.h"
#include "with_evaluators/with_evaluator_factory.h"
#include "join/constraints_solver.h"
#include "tables/table_helpers.h"
#include "string_helpers.h"

namespace qps {

class QueryEvaluator {
 public:
  explicit QueryEvaluator(Query query);

  void EvaluateQuery(QueryFacade &pkb, std::list<std::string> &results);

 private:
  Query query_;
  std::vector<std::unique_ptr<ClauseEvaluator>> clause_evaluators_;
  void CreateClauseEvaluators();
  auto EvaluateClauses(QueryFacade &pkb) -> ClauseResult;
  auto EvaluateSelect(QueryFacade &pkb, ClauseResult clause_result) -> FinalResult;

};
}  // namespace qps
