#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class AssignEvaluator : public PatternEvaluator {
 public:
  AssignEvaluator(PatternClause clause, std::vector<Declaration> declarations)
      : PatternEvaluator(std::move(clause), std::move(declarations)) {}
  auto CallPkb(QueryFacade &pkb) -> std::vector<Product> override;

private:
  static auto checkExpressionContained(const std::vector<Relationship *> &pkb_res,
                                                               const std::string &postfix,
                                                               bool is_partial) -> std::vector<Relationship *>;
};

}  // namespace qps
