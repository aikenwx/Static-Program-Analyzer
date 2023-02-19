#include "uses_evaluator.h"

namespace qps {
std::vector<::Relationship *> UsesEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getUsesRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> UsesEvaluator::GetLeftHandTypes(Ref &left_arg) {
  return {EntityType::STATEMENT, EntityType::PROCEDURE};
}

std::vector<EntityType> UsesEvaluator::GetRightHandTypes(Ref &right_arg) {
  return {EntityType::VARIABLE};
}

}// qps
