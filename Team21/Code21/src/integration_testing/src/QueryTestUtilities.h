#include <unordered_set>
#include <list>

#include "query_evaluators/QPS.h"

std::unordered_set<std::string> RunQuery(std::string query_str, QueryFacade &pkb) {
  std::list<std::string> results;
  qps::QPS::evaluate(query_str, results, pkb);
  std::unordered_set<std::string> result_set(results.begin(), results.end());
  return result_set;
}
