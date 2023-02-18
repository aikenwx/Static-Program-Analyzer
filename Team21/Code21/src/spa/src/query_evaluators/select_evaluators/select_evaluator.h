#pragma once

#include "query_evaluators/clause_evaluator.h"

#include "query/declaration.h"

namespace qps {

class SelectEvaluator : public ClauseEvaluator {
 public:
  explicit SelectEvaluator(Declaration declaration) : declaration_(std::move(declaration)) {}
  ~SelectEvaluator() override = default;

  ClauseResult Evaluate(QueryFacade &pkb) override;
  ClauseResult ConstructResult(const std::vector<Entity *> &entities);

 private:
  Declaration declaration_;
};

} // qps
