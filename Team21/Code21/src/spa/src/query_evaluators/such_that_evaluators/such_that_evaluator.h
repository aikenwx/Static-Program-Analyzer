#pragma once

#include "query_evaluators/clause_evaluator.h"
#include "query/such_that_clause.h"

namespace qps {

class SuchThatEvaluator : public ClauseEvaluator {
 public:
  SuchThatEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
      : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

  SuchThatClause clause_;
  std::vector<Declaration> declarations_;
};

auto GetEntityType(Synonym &syn, std::vector<Declaration> &declarations) -> EntityType;

} // namespace qps
