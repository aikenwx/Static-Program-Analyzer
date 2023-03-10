#pragma once

#include "query_evaluators/clause_evaluator.h"

#include "query/query.h"

namespace qps {

class SelectEvaluator : public ClauseEvaluator {
 public:
  explicit SelectEvaluator(Synonym synonym, std::vector<Declaration> declarations)
      : synonym_(std::move(synonym)), declarations_(std::move(declarations)) {}
  ~SelectEvaluator() override = default;

  ClauseResult Evaluate(QueryFacade &pkb) override;
 private:
  Synonym synonym_;
  std::vector<Declaration> declarations_;
  ClauseResult ConstructResult(const std::vector<Entity *> &entities);
};

} // qps
