#pragma once

#include "unordered_set"

#include "query/query.h"
#include "PKB/QueryFacade.h"

namespace qps {

class QueryEvaluator {
 public:
  QueryEvaluator(Query query) : query_(query) {}

  std::unordered_set<std::string> evaluateQuery(QueryFacade &pkb);

 private:
  Query query_;
};
} // qps
