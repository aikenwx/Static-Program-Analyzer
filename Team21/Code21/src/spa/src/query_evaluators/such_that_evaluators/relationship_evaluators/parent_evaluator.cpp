#include "parent_evaluator.h"

namespace qps {

auto ParentEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> {
  std::vector<::Relationship *> res;
  auto *pkb_res = pkb.getParentRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

auto ParentEvaluator::GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> {
  return RelationshipEvaluator::CONTAINER_TYPES;
}

auto ParentEvaluator::GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> {
  return {Statement::getEntityTypeStatic()};
}
}  // namespace qps
