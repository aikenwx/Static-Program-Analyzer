#pragma once

#include <unordered_set>
#include <list>

#include "query/query.h"
#include "PKB/QueryFacade.h"
#include "clause_evaluator.h"
#include "query_evaluators/select_evaluators/select_evaluator.h"

namespace qps {

class QueryEvaluator {
 public:
  explicit QueryEvaluator(Query query);

  void EvaluateQuery(QueryFacade &pkb, std::list<std::string> &results);

 private:
  Query query_;
  std::vector<std::unique_ptr<ClauseEvaluator>> clause_evaluators_;
  void CreateClauseEvaluators();
  void EvaluateSelect(QueryFacade &pkb, ClauseResult clause_result, std::list<std::string> &results);

};
}  // namespace qps
