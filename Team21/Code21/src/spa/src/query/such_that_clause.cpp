#include <utility>

#include "query/such_that_clause.h"

namespace qps {
auto SuchThatClause::getRelationship() const -> Relationship {
  return relationship;
}

auto SuchThatClause::getArg1() const -> const Ref & {
  return arg1;
}

auto SuchThatClause::getArg2() const -> const Ref & {
  return arg2;
}

SuchThatClause::SuchThatClause(Relationship relationship_, Ref arg1_, Ref arg2_,
                               std::vector<Declaration> &declarations)
    : relationship{relationship_}, arg1{std::move(arg1_)}, arg2{std::move(arg2_)} {
  handleModifiesUses(declarations);
}
}  // namespace qps
