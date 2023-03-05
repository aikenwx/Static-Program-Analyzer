#include "such_that_evaluator_factory.h"

#include "parent_evaluator.h"
#include "follows_evaluator.h"
#include "uses_evaluator.h"
#include "modifies_evaluator.h"
#include "parent_star_evaluator.h"
#include "follows_star_evaluator.h"
#include "calls_evaluator.h"
#include "calls_star_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
std::unique_ptr<SuchThatEvaluator> SuchThatEvaluatorFactory::Create(SuchThatClause &clause,
                                                                    std::vector<Declaration> &decl_lst) {
  switch (clause.getRelationship()) {
    case Relationship::Parent:return std::make_unique<ParentEvaluator>(clause, decl_lst);
    case Relationship::ParentT:return std::make_unique<ParentStarEvaluator>(clause, decl_lst);
    case Relationship::Follows:return std::make_unique<FollowsEvaluator>(clause, decl_lst);
    case Relationship::FollowsT:return std::make_unique<FollowsStarEvaluator>(clause, decl_lst);
    case Relationship::UsesP:return std::make_unique<UsesEvaluator>(clause, decl_lst);
    case Relationship::UsesS:return std::make_unique<UsesEvaluator>(clause, decl_lst);
    case Relationship::Uses:return std::make_unique<UsesEvaluator>(clause, decl_lst);
    case Relationship::ModifiesP:return std::make_unique<ModifiesEvaluator>(clause, decl_lst);
    case Relationship::ModifiesS:return std::make_unique<ModifiesEvaluator>(clause, decl_lst);
    case Relationship::Modifies:return std::make_unique<ModifiesEvaluator>(clause, decl_lst);
    case Relationship::Calls:return std::make_unique<CallsEvaluator>(clause, decl_lst);
    case Relationship::CallsT:return std::make_unique<CallsStarEvaluator>(clause, decl_lst);
    default:throw std::invalid_argument("Clause type not supported yet");
  }
}
} // qps
