#include <numeric>

#include "constraints_solver.h"
#include "join.h"

namespace qps {
auto ConstraintsSolver::solve(const std::vector<SynonymTable> &evaluators) -> SynonymTable {
  if (evaluators.size() == 1) {
    return evaluators[0];
  }
  auto join = [](const SynonymTable &table1, const SynonymTable &table2) {
    return HashJoin<SynonymTable>{}(table1, table2);
  };
  return std::accumulate(std::next(evaluators.begin()), evaluators.end(), evaluators[0], join);
}
}  // namespace qps
