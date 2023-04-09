#pragma once

#include "query/with_clause.h"
#include "with_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class WithIntAttrEvaluator : public WithEvaluator {
 public:
  WithIntAttrEvaluator(const WithClause &clause, const std::vector<Declaration> &declarations)
      : WithEvaluator(clause, declarations) {}
  auto CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>> override;
};

} // namespace qps
