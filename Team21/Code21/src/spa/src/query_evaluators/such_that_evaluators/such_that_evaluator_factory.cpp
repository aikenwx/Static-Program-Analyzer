#include "such_that_evaluator_factory.h"
#include "parent_evaluator.h"
#include "follows_evaluator.h"
#include "uses_evaluator.h"
#include "modifies_evaluator.h"

#include "query/query_exceptions.h"

namespace qps {
std::unique_ptr<SuchThatEvaluator> SuchThatEvaluatorFactory::Create(SuchThatClause &clause,
                                                                    std::vector<Declaration> &decl_lst) {
  switch (clause.getRelationship()) {
    case Relationship::Parent:return std::make_unique<ParentEvaluator>(clause, decl_lst);
    case Relationship::Follows:return std::make_unique<FollowsEvaluator>(clause, decl_lst);
    case Relationship::UsesP:
    case Relationship::UsesS:
    case Relationship::Uses:return std::make_unique<UsesEvaluator>(clause, decl_lst);
    case Relationship::ModifiesP:
    case Relationship::ModifiesS:
    case Relationship::Modifies:return std::make_unique<ModifiesEvaluator>(clause, decl_lst);
    default:throw std::invalid_argument("Clause type not supported yet");
  }
}
} // qps
