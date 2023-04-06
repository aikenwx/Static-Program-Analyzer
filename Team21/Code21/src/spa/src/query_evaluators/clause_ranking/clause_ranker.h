#pragma once

#include "query/such_that_clause.h"
#include "query/with_clause.h"
#include "query/pattern_clause.h"
namespace qps {

class ClauseRanker {
  static void SortSuchThatClause(std::vector<SuchThatClause> &);
  static void SortPatternClause(std::vector<PatternClause> &);
  static void SortWithClause(std::vector<WithClause> &);
};

} // qps
