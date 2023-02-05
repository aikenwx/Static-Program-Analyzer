#pragma once

#include "query_evaluators/clause_evaluator.h"
#include "query/declaration.h"

namespace qps {

class SelectEvaluator : public ClauseEvaluator {
 public:
  SelectEvaluator(Declaration declaration) : declaration_(std::move(declaration)) {}
  ~SelectEvaluator() override=default;

  ClauseResult evaluate(QueryFacade &pkb) override;
  ClauseResult ConstructResult(const std::vector<Entity *>& entities);

 private:
  Declaration declaration_;
};

} // qps
