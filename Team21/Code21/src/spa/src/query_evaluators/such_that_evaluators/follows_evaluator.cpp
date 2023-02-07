#include "follows_evaluator.h"

namespace qps {
std::vector<::Relationship *> FollowsEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getFollowsRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> FollowsEvaluator::GetLeftHandTypes(Ref &left_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&left_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return {EntityType::STATEMENT};
  }
}

// TODO: Refactor some repetition of code
std::vector<EntityType> FollowsEvaluator::GetRightHandTypes(Ref &right_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&right_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return {EntityType::STATEMENT};
  }
}

} // qps
