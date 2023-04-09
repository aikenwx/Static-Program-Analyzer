#pragma once

#include "query_evaluators/clause_evaluator.h"
#include "query/query.h"
#include "query_evaluators/select_evaluators/attr_ref_eval.h"
#include "query_evaluators/join/constraints_solver.h"

#include <list>

namespace qps {
class SelectEvaluator {
 public:
  SelectEvaluator(Result result, std::vector<Declaration> declarations)
      : result_(std::move(result)), declarations_(std::move(declarations)) {}

  void Evaluate(QueryFacade &pkb, ClauseResults current_result, std::list<std::string> &results);
 private:
  Result result_;
  std::vector<Declaration> declarations_;
};

}  // namespace qps
