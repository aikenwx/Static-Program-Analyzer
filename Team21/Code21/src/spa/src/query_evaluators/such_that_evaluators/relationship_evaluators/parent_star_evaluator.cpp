#include "parent_star_evaluator.h"

namespace qps {
std::vector<::Relationship *> ParentStarEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getParentStarRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> ParentStarEvaluator::GetLeftHandTypes(Ref &left_arg) {
  return {WhileStatement::getEntityTypeStatic(), IfStatement::getEntityTypeStatic()};
}

std::vector<EntityType> ParentStarEvaluator::GetRightHandTypes(Ref &right_arg) {
  return {Statement::getEntityTypeStatic()};
}
} // qps
