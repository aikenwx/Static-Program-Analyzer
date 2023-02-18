#pragma once

#include <unordered_set>
#include <list>

#include "query_evaluators/QPS.h"
namespace qps_test {
std::unordered_set<std::string> RunQuery(std::string query_str, QueryFacade &pkb);
} // qps_test
