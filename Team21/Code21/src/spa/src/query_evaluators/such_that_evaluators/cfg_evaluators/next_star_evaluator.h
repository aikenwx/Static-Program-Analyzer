#pragma once

#include "cfg_evaluator.h"

namespace qps {

class NextStarEvaluator : public Cfg_Evaluator {
 public:
  NextStarEvaluator(SuchThatClause clause, std::vector<Declaration> declarations) :
      Cfg_Evaluator(std::move(clause), std::move(declarations)) {}

  ClauseResult Evaluate(QueryFacade &pkb) override;

};

} // namespace qps
