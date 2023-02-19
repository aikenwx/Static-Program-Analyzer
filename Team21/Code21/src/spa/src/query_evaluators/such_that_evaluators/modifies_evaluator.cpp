#include "modifies_evaluator.h"

namespace qps {
std::vector<::Relationship *> ModifiesEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getModifiesRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> ModifiesEvaluator::GetLeftHandTypes(Ref &left_arg) {
  return {EntityType::STATEMENT, EntityType::PROCEDURE};
}

std::vector<EntityType> ModifiesEvaluator::GetRightHandTypes(Ref &right_arg) {
  return {EntityType::VARIABLE};
}

} // qps
