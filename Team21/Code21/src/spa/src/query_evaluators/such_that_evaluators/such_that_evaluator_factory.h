#pragma once

#include "memory"

#include "query/such_that_clause.h"
#include "such_that_evaluator.h"

namespace qps {

class SuchThatEvaluatorFactory {
 public:
  static std::unique_ptr<SuchThatEvaluator> Create(SuchThatClause &clause, std::vector<Declaration> &decl_lst);
};

} // qps
