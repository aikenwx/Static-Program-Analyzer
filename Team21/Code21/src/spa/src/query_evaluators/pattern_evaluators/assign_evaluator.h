#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class AssignEvaluator : public PatternEvaluator {
 public:
  AssignEvaluator(const PatternClause &clause, const std::vector<Declaration> &declarations)
      : PatternEvaluator(clause, declarations) {}
  auto CallPkb(QueryFacade &pkb) -> std::vector<Product> override;

 private:
  static auto checkExpressionContained(const std::vector<ModifiesRelationship *> &pkb_res,
                                       const std::string &postfix,
                                       bool is_partial) -> std::vector<ModifiesRelationship *>;
};

}  // namespace qps
