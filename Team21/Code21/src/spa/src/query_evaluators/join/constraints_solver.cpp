#include <numeric>

#include "constraints_solver.h"
#include "join.h"

namespace qps {
auto ConstraintsSolver::solve(std::vector<SynonymTable> tables) -> std::optional<SynonymTable> {
  if (tables.empty()) {
    return std::nullopt;
  }
  auto joiner = HashJoin<SynonymTable>{};
  auto curr_table = std::move(tables[0]);
  for (int i = 1; i < tables.size(); ++i) {
    curr_table = joiner(curr_table, tables[i]);
    if (!curr_table) {
      return std::nullopt;
    }
  }
  return curr_table;
}

auto ConstraintsSolver::MergeTableWithGrps(SynonymTable table) -> SynonymTable {
  auto joiner = HashJoin<SynonymTable>{};
  for (auto grp : GetNei(table)) {
    table = joiner(table, grp_table[grp]);
    if (!table) {
      return table;
    }
    grp_table.erase(grp);
  }
  return table;
}

auto ConstraintsSolver::GetNei(const SynonymTable &table) -> std::unordered_set<int> {
  std::unordered_set<int> nei_grps;
  for (const auto &syn : table.GetHeader()) {
    auto grp = syn_grp.find(syn);
    if (grp == syn_grp.end()) {
      continue;
    }
    nei_grps.insert(grp->second);
  }
  return nei_grps;
}

void ConstraintsSolver::UpdateSynGrp(SynonymTable table) {
  for (const auto &syn : table.GetHeader()) {
    syn_grp[syn] = num_grps;
  }
  grp_table[num_grps] = std::move(table);
  num_grps++;
}

auto ConstraintsSolver::MergeDisjointGrps() -> ClauseResult {
  if (grp_table.empty()) {
    return true;
  }
  std::vector<SynonymTable> tables;
  tables.reserve(grp_table.size());
  for (auto &iter : grp_table) {
    tables.push_back(std::move(iter.second));
  }
  auto table = ConstraintsSolver::solve(std::move(tables));
  if (!table) {
    return false;
  }
  auto res = std::move(table.value());
  return res;
}

auto ConstraintsSolver::solve(std::vector<std::unique_ptr<ClauseEvaluator>> &evaluators) -> ClauseResult {
  for (const auto &evaluator : evaluators) {
    auto result = evaluator->Evaluate(pkb_);
    if (std::holds_alternative<bool>(result)) {
      if (!std::get<bool>(result)) {
        return false;
      }
      continue;
    }
    auto table = std::get<SynonymTable>(std::move(result));
    table = MergeTableWithGrps(std::move(table));
    if (!table) {
      return false;
    }
    UpdateSynGrp(std::move(table));
  }
  if (grp_table.empty()) {
    return true;
  }
  return MergeDisjointGrps();
}
}  // namespace qps
