#pragma once

#include "unordered_set"

#include "query/query.h"
#include "PKB/QueryFacade.h"
#include "clause_evaluator.h"

namespace qps {

class QueryEvaluator {
 public:
  QueryEvaluator(Query query);

  std::unordered_set<std::string> EvaluateQuery(QueryFacade &pkb);

 private:
  Query query_;
  bool early_return_ = false;
  std::vector<std::unique_ptr<ClauseEvaluator>> clause_evaluators_;
  std::unique_ptr<ClauseEvaluator> select_evalautor_;
  std::vector<SynonymTable> tables_;
  void CreateEvaluators();
  void EvaluateClauses(QueryFacade &pkb);
  std::unordered_set<std::string> EvaluateSelect(QueryFacade &pkb);

};
} // qps
