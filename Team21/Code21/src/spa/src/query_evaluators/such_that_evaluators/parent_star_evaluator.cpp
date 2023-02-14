#include "parent_star_evaluator.h"

namespace qps {
std::vector<::Relationship *> ParentStarEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getParentStarRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> ParentStarEvaluator::GetLeftHandTypes(Ref &left_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&left_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return {EntityType::WHILE_STATEMENT, EntityType::IF_STATEMENT};
  }
}

// TODO: Refactor some repetition of code
std::vector<EntityType> ParentStarEvaluator::GetRightHandTypes(Ref &right_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&right_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return {EntityType::STATEMENT};
  }
}
} // qps
