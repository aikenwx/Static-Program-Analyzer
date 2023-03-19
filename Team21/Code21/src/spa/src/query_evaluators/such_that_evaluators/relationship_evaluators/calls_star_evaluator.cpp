#include "calls_star_evaluator.h"

namespace qps {
auto CallsStarEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> {
  std::vector<::Relationship *> res;
  auto *pkb_res = pkb.getAllCallsStarRelationships();
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

auto CallsStarEvaluator::GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> {
  return {Procedure::getEntityTypeStatic()};
}

auto CallsStarEvaluator::GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> {
  return {Procedure::getEntityTypeStatic()};
}
}  // namespace qps
