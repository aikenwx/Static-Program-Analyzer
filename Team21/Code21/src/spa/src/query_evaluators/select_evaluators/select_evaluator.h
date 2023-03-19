#pragma once

#include "query_evaluators/clause_evaluator.h"
#include "query/query.h"

namespace qps {
using FinalTable = RowTable<Synonym, std::string>;
using FinalResult = std::variant<bool, FinalTable>;
class SelectEvaluator {
 public:
  SelectEvaluator(Result result, std::vector<Declaration> declarations)
      : result_(std::move(result)), declarations_(std::move(declarations)) {}

  auto Evaluate(QueryFacade &pkb, ClauseResult current_result) -> FinalResult;
 private:
  Result result_;
  std::vector<Declaration> declarations_;
};

}  // namespace qps
