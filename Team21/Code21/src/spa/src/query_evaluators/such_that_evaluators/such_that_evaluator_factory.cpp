#include "such_that_evaluator_factory.h"
#include "parent_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
std::unique_ptr<SuchThatEvaluator> SuchThatEvaluatorFactory::Create(SuchThatClause &clause,
                                                                    std::vector<Declaration> &decl_lst) {
  switch (clause.getRelationship()) {
    case Relationship::Parent:return std::make_unique<ParentEvaluator>(clause, decl_lst);
    default:throw QueryException("Clause type not supported yet");
  }

}
} // qps
