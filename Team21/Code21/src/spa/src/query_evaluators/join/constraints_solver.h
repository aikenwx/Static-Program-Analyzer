#pragma once

#include "query_evaluators/synonym_table.h"

namespace qps {
class ConstraintsSolver {
 public:
  static SynonymTable solve(const std::vector<SynonymTable> &evaluators);
};
}
