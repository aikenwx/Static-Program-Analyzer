#include "calls_evaluator.h"

namespace qps {
auto CallsEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> {
  std::vector<::Relationship *> res;
  auto *pkb_res = pkb.getAllCallsRelationships();
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

auto CallsEvaluator::GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> {
  return {Procedure::getEntityTypeStatic()};
}

auto CallsEvaluator::GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> {
  return {Procedure::getEntityTypeStatic()};
}

}  // namespace qps
