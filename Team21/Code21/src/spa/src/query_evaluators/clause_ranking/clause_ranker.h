#pragma once

#include "query/such_that_clause.h"
#include "query/with_clause.h"
#include "query/pattern_clause.h"
#include "query/query.h"
namespace qps {

class ClauseRanker {
 public:
  explicit ClauseRanker(const std::vector<Declaration> &decl_lst) : decl_lst_(decl_lst) {}
  void SortQuery(Query &);
  void SortSuchThatClause(std::vector<SuchThatClause> &);
  void SortPatternClause(std::vector<PatternClause> &);
  void SortWithClause(std::vector<WithClause> &);

 private:
  const std::vector<Declaration> &decl_lst_;
};

}  // namespace qps
