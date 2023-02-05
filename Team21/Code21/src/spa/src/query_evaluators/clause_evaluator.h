#pragma once

#include "clause_result.h"

#include "PKB/QueryFacade.h"

namespace qps {

class ClauseEvaluator {
 public:
  virtual ClauseResult evaluate(QueryFacade &pkb) = 0;
  virtual ~ClauseEvaluator() = default;
};
} // qps

