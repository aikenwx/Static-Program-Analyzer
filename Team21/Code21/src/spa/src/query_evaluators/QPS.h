#pragma once

#include <list>
#include <string>

#include "PKB/QueryFacade.h"

namespace qps {

class QPS {
 public:
  static void evaluate(std::string &query_str, std::list<std::string> &results, QueryFacade &pkb);

};

} // qps
