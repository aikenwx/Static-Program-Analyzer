#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class IfEvaluator : public PatternEvaluator {
 public:
  IfEvaluator(const PatternClause &clause, const std::vector<Declaration> &declarations)
      : PatternEvaluator(clause, declarations) {}
  auto CallPkb(QueryFacade &pkb) -> std::vector<Product> override;
};

} // namespace qps
