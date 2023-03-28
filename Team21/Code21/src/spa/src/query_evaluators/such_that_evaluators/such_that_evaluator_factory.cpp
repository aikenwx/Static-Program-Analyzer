#include "such_that_evaluator_factory.h"

#include "query_evaluators/such_that_evaluators/relationship_evaluators/relationship_evaluator.h"
#include "query_evaluators/such_that_evaluators/cfg_evaluators/next_evaluator.h"
#include "query_evaluators/such_that_evaluators/cfg_evaluators/next_star_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
auto SuchThatEvaluatorFactory::Create(SuchThatClause &clause,
                                      std::vector<Declaration> &decl_lst) -> std::unique_ptr<SuchThatEvaluator> {
  switch (clause.getRelationship()) {
    case Relationship::Parent:
    case Relationship::ParentT:
    case Relationship::Follows:
    case Relationship::FollowsT:
    case Relationship::UsesP:
    case Relationship::UsesS:
    case Relationship::Uses:
    case Relationship::ModifiesP:
    case Relationship::ModifiesS:
    case Relationship::Modifies:
    case Relationship::Calls:
    case Relationship::CallsT:return std::make_unique<RelationshipEvaluator>(clause, decl_lst);
    case Relationship::Next:return std::make_unique<NextEvaluator>(clause, decl_lst);
    case Relationship::NextT:return std::make_unique<NextStarEvaluator>(clause, decl_lst);
    default:throw std::invalid_argument("Clause type not supported yet");
  }
}
}  // namespace qps
