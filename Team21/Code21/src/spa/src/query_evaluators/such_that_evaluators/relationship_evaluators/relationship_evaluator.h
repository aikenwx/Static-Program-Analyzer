#pragma once

#include "query/declaration.h"
#include "query/such_that_clause.h"
#include "query_evaluators/such_that_evaluators/such_that_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

namespace qps {

class RelationshipEvaluator : public SuchThatEvaluator {
 public:
  RelationshipEvaluator(SuchThatClause clause, std::vector<Declaration> declarations) :
      SuchThatEvaluator(std::move(clause), std::move(declarations)) {}

  auto Evaluate(QueryFacade &pkb) -> ClauseResult override;

 private:
};

}  // namespace qps
