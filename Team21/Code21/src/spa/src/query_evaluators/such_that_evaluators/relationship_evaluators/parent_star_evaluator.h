#include "query/such_that_clause.h"
#include "relationship_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class ParentStarEvaluator : public RelationshipEvaluator {
 public:
  ParentStarEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
      : RelationshipEvaluator(std::move(clause), std::move(declarations)) {}
  auto CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> override;
  auto GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> override;
  auto GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> override;
};

}  // namespace qps
