#include "query/query_exceptions.h"
#include "query_evaluator.h"
#include "select_evaluators/select_evaluator.h"
#include "such_that_evaluators/such_that_evaluator_factory.h"
#include "pattern_evaluators/pattern_evaluator_factory.h"
#include "join/constraints_solver.h"

namespace qps {
QueryEvaluator::QueryEvaluator(Query query) : query_(std::move(query)), early_return_(false) {
  CreateEvaluators();
}

Synonym getSynonynm(Query &query) {
  auto elements = std::get<std::vector<Element>>(query.getSelectClause());
  return std::get<Synonym>(elements[0]);
}
void QueryEvaluator::CreateEvaluators() {
  auto declarations = query_.getDeclarations();
  select_evalautor_ = std::make_unique<SelectEvaluator>(getSynonynm(query_), declarations);
  for (auto &clause : query_.getSuchThatClause()) {
    clause_evaluators_.push_back(SuchThatEvaluatorFactory::Create(clause, declarations));
  }
  for (auto &clause : query_.getPatternClause()) {
    clause_evaluators_.push_back(PatternEvaluatorFactory::Create(clause, declarations));
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
  auto target = getSynonynm(query_);
  if (tables_.empty()) {
    return std::get<SynonymTable>(select_evalautor_->Evaluate(pkb)).Extract(target);
  }
  auto final_table = ConstraintsSolver::solve(tables_);
  if (final_table.Empty()) {
    return {};
  }
  if (!final_table.HasSynonym(target)) {
    final_table = std::get<SynonymTable>(select_evalautor_->Evaluate(pkb));
  }
  return final_table.Extract(target);
}

std::unordered_set<std::string> QueryEvaluator::EvaluateQuery(QueryFacade &pkb) {
  EvaluateClauses(pkb);
  if (early_return_) return {};
  return EvaluateSelect(pkb);
}
} // qps
