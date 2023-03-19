#pragma once

#include "memory"

#include "query/such_that_clause.h"
#include "query_evaluators/such_that_evaluators/relationship_evaluators/relationship_evaluator.h"

namespace qps {

class SuchThatEvaluatorFactory {
 public:
  static auto Create(SuchThatClause &clause, std::vector<Declaration> &decl_lst) -> std::unique_ptr<SuchThatEvaluator>;
};

} // qps
