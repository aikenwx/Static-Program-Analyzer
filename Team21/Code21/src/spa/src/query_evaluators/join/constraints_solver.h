#pragma once

#include "query_evaluators/clause_evaluator.h"

#include <optional>
#include <unordered_map>
#include <vector>

namespace qps {
struct ClauseGrps {
  std::unordered_map<Synonym, int> syn_grp;
  std::unordered_map<int, SynonymTable> grp_table;
};

using ClauseResults = std::variant<bool, ClauseGrps>;

class ConstraintsSolver {
 public:
  static auto solve(std::vector<SynonymTable> tables) -> std::optional<SynonymTable>;

  explicit ConstraintsSolver(QueryFacade &pkb) : pkb_(pkb) {}
  auto solve(std::vector<std::unique_ptr<ClauseEvaluator>> &evaluators) -> ClauseResults;
 private:
  QueryFacade &pkb_;
  std::unordered_map<Synonym, int> syn_grp;
  std::unordered_map<int, SynonymTable> grp_table;
  int num_grps;

  auto MergeTableWithGrps(SynonymTable table) -> SynonymTable;
  auto GetNei(const SynonymTable &table) -> std::unordered_set<int>;
  void UpdateSynGrp(SynonymTable table);
};
}  // namespace qps
