#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "pattern_evaluators/pattern_evaluator_factory.h"
#include "with_evaluators/with_evaluator_factory.h"
#include "join/constraints_solver.h"
#include "tables/table_helpers.h"
#include "string_helpers.h"
#include "query_evaluators/such_that_evaluators/such_that_evaluator.h"

namespace qps {
QueryEvaluator::QueryEvaluator(Query query) : query_(std::move(query)) {}

void QueryEvaluator::CreateClauseEvaluators() {
  auto declarations = query_.getDeclarations();
  for (auto &clause : query_.getSuchThatClause()) {
    clause_evaluators_.push_back(std::make_unique<SuchThatEvaluator>(clause, declarations));
  }
  for (auto &clause : query_.getPatternClause()) {
    clause_evaluators_.push_back(PatternEvaluatorFactory::Create(clause, declarations));
  }
  for (auto &clause : query_.getWithClause()) {
    clause_evaluators_.push_back(WithEvaluatorFactory::Create(clause, declarations));
  }
}

auto QueryEvaluator::EvaluateClauses(QueryFacade &pkb) -> ClauseResult {
  CreateClauseEvaluators();
  if (clause_evaluators_.empty()) {
    return true;
  }
  std::vector<SynonymTable> tables;
  for (const auto &evaluator : clause_evaluators_) {
    auto res = evaluator->Evaluate(pkb);
    if (std::holds_alternative<bool>(res)) {
      if (!std::get<bool>(res)) {
        return false;
      }
    } else {
      auto table = std::get<SynonymTable>(res);
      if (table.IsEmpty()) {
        return false;
      }
      tables.push_back(std::move(table));
    }
  }
  if (tables.empty()) {
    return true;
  }
  auto final_table = ConstraintsSolver::solve(tables);
  if (final_table.IsEmpty()) {
    return false;
  }
  return final_table;
}

auto QueryEvaluator::EvaluateSelect(QueryFacade &pkb, ClauseResult clause_result) -> FinalResult {
  SelectEvaluator select_evaluator{query_.getSelectClause(), query_.getDeclarations()};
  auto result = select_evaluator.Evaluate(pkb, std::move(clause_result));
  return result;

}

void QueryEvaluator::EvaluateQuery(QueryFacade &pkb, std::list<std::string> &results) {
  auto clause_result = EvaluateClauses(pkb);
  auto final_result = EvaluateSelect(pkb, std::move(clause_result));
  if (std::holds_alternative<bool>(final_result)) {
    results.emplace_back(to_string(std::get<bool>(final_result)));
  } else {
    std::unordered_set<std::string> seen;
    auto final_table = std::get<FinalTable>(final_result);
    for (const auto &row : final_table.GetRows()) {
      auto result = Join(row, " ");
      seen.insert(result);
    }
    results.insert(results.end(), seen.begin(), seen.end());
  }
}
}  // namespace qps
