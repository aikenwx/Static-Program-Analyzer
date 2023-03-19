#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

  class WhileEvaluator : public PatternEvaluator {
  public:
    WhileEvaluator(PatternClause clause, std::vector<Declaration> declarations)
      : PatternEvaluator(std::move(clause), std::move(declarations)) {}
    auto CallPkb(QueryFacade& pkb) -> std::vector<Product> override;
  };

} // namespace qps
