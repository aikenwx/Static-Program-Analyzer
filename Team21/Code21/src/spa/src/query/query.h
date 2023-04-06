#pragma once

#include "query/declaration.h"
#include "query/pattern_clause.h"
#include "query/such_that_clause.h"
#include "query/element.h"
#include "query/boolean.h"
#include "query/with_clause.h"

#include <vector>

namespace qps {

using Clause = std::variant<qps::SuchThatClause, qps::PatternClause>;
using Result = std::variant<std::vector<Element>, Boolean>;

class Query {
 private:
  std::vector<Declaration> declarations;
  std::vector<SuchThatClause> suchThatClause;
  std::vector<PatternClause> patternClause;
  std::vector<WithClause> withClause;
  Result selectClause;

 public:
  Query(std::vector<Declaration> declarations_, std::vector<SuchThatClause> suchThatClause_,
        std::vector<PatternClause> patternClause_, std::vector<WithClause> withClause_, Result selectClause_);
  explicit Query(Result selectClause_);
  auto getDeclarations() -> std::vector<Declaration> &;
  auto getSuchThatClause() -> std::vector<SuchThatClause> &;
  auto getPatternClause() -> std::vector<PatternClause> &;
  auto getWithClause() -> std::vector<WithClause> &;
  auto getSelectClause() -> Result &;
};
}  // namespace qps
