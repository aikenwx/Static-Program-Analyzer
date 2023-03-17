#include "calls_star_evaluator.h"

namespace qps {
std::vector<::Relationship *> CallsStarEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getAllCallsStarRelationships();
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> CallsStarEvaluator::GetLeftHandTypes(Ref &left_arg) {
  return {Procedure::getEntityTypeStatic()};
}

std::vector<EntityType> CallsStarEvaluator::GetRightHandTypes(Ref &right_arg) {
  return {Procedure::getEntityTypeStatic()};
}
} // qps
