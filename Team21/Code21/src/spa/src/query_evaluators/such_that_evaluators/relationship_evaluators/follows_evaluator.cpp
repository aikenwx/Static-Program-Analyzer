#include "follows_evaluator.h"

namespace qps {
auto FollowsEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> {
  std::vector<::Relationship *> res;
  auto *pkb_res = pkb.getFollowsRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

auto FollowsEvaluator::GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> {
  return {Statement::getEntityTypeStatic()};
}

auto FollowsEvaluator::GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> {
  return {Statement::getEntityTypeStatic()};
}

}  // namespace qps
