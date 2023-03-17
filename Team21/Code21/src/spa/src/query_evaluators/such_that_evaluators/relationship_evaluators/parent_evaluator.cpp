
#include "query/query_exceptions.h"
#include "parent_evaluator.h"

namespace qps {

std::vector<::Relationship *> ParentEvaluator::CallPkb(QueryFacade &pkb, EntityType left, EntityType right) {
  std::vector<::Relationship *> res;
  auto pkb_res = pkb.getParentRelationshipsByLeftAndRightEntityTypes(left, right);
  res.assign(pkb_res->begin(), pkb_res->end());
  return res;
}

std::vector<EntityType> ParentEvaluator::GetLeftHandTypes(Ref &left_arg) {
  return RelationshipEvaluator::CONTAINER_TYPES;
}

std::vector<EntityType> ParentEvaluator::GetRightHandTypes(Ref &right_arg) {
  return {Statement::getEntityTypeStatic()};
}

} // qps
