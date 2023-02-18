#include "modifies_evaluator.h"

namespace qps {
std::vector<::Relationship *> ModifiesEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getModifiesRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> ModifiesEvaluator::GetLeftHandTypes(Ref &left_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&left_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return {EntityType::STATEMENT, EntityType::PROCEDURE};
  }
}

// TODO: Refactor some repetition of code
std::vector<EntityType> ModifiesEvaluator::GetRightHandTypes(Ref &right_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&right_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return {EntityType::VARIABLE};
  }
}

} // qps
