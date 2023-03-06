#include "calls_evaluator.h"

namespace qps {
std::vector<::Relationship *> CallsEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getAllCallsRelationships();
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> CallsEvaluator::GetLeftHandTypes(Ref &left_arg) {
  return {EntityType::PROCEDURE};
}

std::vector<EntityType> CallsEvaluator::GetRightHandTypes(Ref &right_arg) {
  return {EntityType::PROCEDURE};
}

} // qps
