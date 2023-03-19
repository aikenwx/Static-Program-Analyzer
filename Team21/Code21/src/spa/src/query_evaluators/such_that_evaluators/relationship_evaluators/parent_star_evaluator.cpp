#include "parent_star_evaluator.h"

namespace qps {
auto ParentStarEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> {
  std::vector<::Relationship *> res;
  auto *pkb_res = pkb.getParentStarRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

auto ParentStarEvaluator::GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> {
  return {WhileStatement::getEntityTypeStatic(), IfStatement::getEntityTypeStatic()};
}

auto ParentStarEvaluator::GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> {
  return {Statement::getEntityTypeStatic()};
}
}  // namespace qps
