#include "query/query_exceptions.h"
#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "such_that_evaluators/such_that_evaluator_factory.h"
#include "join/constraints_solver.h"
namespace qps {
QueryEvaluator::QueryEvaluator(Query query) : query_(std::move(query)), early_return_(false) {
  CreateEvaluators();
}
void QueryEvaluator::CreateEvaluators() {
  auto declarations = query_.getDeclarations();
  select_evalautor_ = std::make_unique<SelectEvaluator>(query_.getSelectClause());
  for (auto &clause : query_.getSuchThatClause()) {
    clause_evaluators_.push_back(SuchThatEvaluatorFactory::Create(clause, declarations));
  }
  for (auto &clause : query_.getPatternClause()) {
    // add in assignment evaluator here
  }
}

void QueryEvaluator::EvaluateClauses(QueryFacade &pkb) {
  for (const auto &evaluator : clause_evaluators_) {
    auto res = evaluator->Evaluate(pkb);
    if (std::holds_alternative<bool>(res)) {
      if (!std::get<bool>(res)) early_return_ = true;
    } else {
      auto table = std::get<SynonymTable>(res);
      if (table.Empty()) early_return_ = true;
      else tables_.push_back(std::move(table));
    }
  }
}

std::unordered_set<std::string> QueryEvaluator::EvaluateSelect(QueryFacade &pkb) {
  auto target = query_.getSelectClause().getSynonym();
  auto final_table =
      tables_.empty() ? std::get<SynonymTable>(select_evalautor_->Evaluate(pkb)) : ConstraintsSolver::solve(tables_);
  if (final_table.Empty() || !final_table.HasSynonym(target)) {
    return {};
  } else {
    return final_table.Extract(target);
  }
}

std::unordered_set<std::string> QueryEvaluator::EvaluateQuery(QueryFacade &pkb) {
  EvaluateClauses(pkb);
  if (early_return_) return {};
  return EvaluateSelect(pkb);
}
} // qps
