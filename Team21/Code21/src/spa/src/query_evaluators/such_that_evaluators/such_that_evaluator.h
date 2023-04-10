#pragma once

#include "query_evaluators/clause_evaluator.h"
#include "query/such_that_clause.h"

namespace qps {

class SuchThatEvaluator : public ClauseEvaluator {
 public:
  SuchThatEvaluator(const SuchThatClause &clause, const std::vector<Declaration> &declarations)
      : clause_(clause), declarations_(declarations) {}

  auto Evaluate(QueryFacade &pkb) -> ClauseResult override;
 private:
  const SuchThatClause &clause_;
  const std::vector<Declaration> &declarations_;
};
} // namespace qps
