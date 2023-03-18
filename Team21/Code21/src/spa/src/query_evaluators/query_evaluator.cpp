#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "such_that_evaluators/such_that_evaluator_factory.h"
#include "pattern_evaluators/pattern_evaluator_factory.h"
#include "join/constraints_solver.h"
#include "tables/table_helpers.h"
#include "string_helpers.h"

namespace qps {
QueryEvaluator::QueryEvaluator(Query query) : query_(std::move(query)) {}

void QueryEvaluator::CreateClauseEvaluators() {
  auto declarations = query_.getDeclarations();
  for (auto &clause : query_.getSuchThatClause()) {
    clause_evaluators_.push_back(SuchThatEvaluatorFactory::Create(clause, declarations));
  }
  for (auto &clause : query_.getPatternClause()) {
    clause_evaluators_.push_back(PatternEvaluatorFactory::Create(clause, declarations));
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
    auto final_table = std::get<FinalTable>(final_result);
    for (const auto &row : final_table.GetRows()) {
      results.emplace_back(Join(row, " "));
    }
  }
}
}  // namespace qps
