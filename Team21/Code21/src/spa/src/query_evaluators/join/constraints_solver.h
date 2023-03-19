#pragma once

#include "query_evaluators/clause_evaluator.h"

namespace qps {
class ConstraintsSolver {
 public:
  static auto solve(const std::vector<SynonymTable> &evaluators) -> SynonymTable;
};
}
