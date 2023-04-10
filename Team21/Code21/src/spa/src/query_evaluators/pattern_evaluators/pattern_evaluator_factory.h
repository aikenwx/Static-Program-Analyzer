#pragma once

#include "memory"

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"

namespace qps {

class PatternEvaluatorFactory {
 public:
  static auto Create(const PatternClause &clause, const std::vector<Declaration> &decl_lst) -> std::unique_ptr<
      PatternEvaluator>;
};

} // namespace qps
