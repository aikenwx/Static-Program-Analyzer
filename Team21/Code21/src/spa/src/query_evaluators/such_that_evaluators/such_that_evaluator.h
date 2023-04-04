#pragma once

#include "query_evaluators/clause_evaluator.h"
#include "query/such_that_clause.h"

namespace qps {

class SuchThatEvaluator : public ClauseEvaluator {
 public:
  SuchThatEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
      : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

  auto Evaluate(QueryFacade &pkb) -> ClauseResult override;
 private:
  SuchThatClause clause_;
  std::vector<Declaration> declarations_;
};
} // namespace qps
