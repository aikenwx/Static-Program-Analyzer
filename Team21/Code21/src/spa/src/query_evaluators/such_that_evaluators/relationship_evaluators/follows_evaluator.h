#pragma once

#include "query/such_that_clause.h"
#include "relationship_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class FollowsEvaluator : public RelationshipEvaluator {
 public:
  FollowsEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
      : RelationshipEvaluator(std::move(clause), std::move(declarations)) {}
  auto CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> override;
  auto GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> override;
  auto GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> override;
};

}  // namespace qps
