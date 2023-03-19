#include "modifies_evaluator.h"

namespace qps {
auto ModifiesEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> {
  std::vector<::Relationship *> res;
  auto *pkb_res = pkb.getModifiesRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

auto ModifiesEvaluator::GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> {
  return {Statement::getEntityTypeStatic(), Procedure::getEntityTypeStatic()};
}

auto ModifiesEvaluator::GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> {
  return {Variable::getEntityTypeStatic()};
}

}  // namespace qps
