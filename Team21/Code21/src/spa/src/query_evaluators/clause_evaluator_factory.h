#pragma once

#include <vector>
#include <memory>

#include "clause_evaluator.h"
#include "query/query.h"

namespace qps {
class ClauseEvaluatorFactory {
  using Evaluators = std::vector<std::unique_ptr<ClauseEvaluator>>;
 public:
  static Evaluators CreateClauseEvaluators(Query &query);
};
} // qps
